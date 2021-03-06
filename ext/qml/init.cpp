#include "ext_metaobject.h"
#include "ext_pointer.h"
#include "ext_pluginloader.h"
#include "ext_anywrapper.h"
#include "ext_accesswrapperfactory.h"
#include "ext_testutil.h"
#include "ext_kernel.h"
#include "kernel.h"
#include "signalforwarder.h"
#include "valuereference.h"
#include "objectgc.h"
#include "listmodel.h"
#include <QtCore/QSet>

using namespace RubyQml;

namespace {

void defineMetaTypes()
{
    qRegisterMetaType<const QMetaObject *>();
}

void defineClasses()
{
    Ext_MetaObject::defineClass();
    Ext_Pointer::defineClass();
    Ext_PluginLoader::defineClass();
    Ext_AnyWrapper::defineClass();
    Ext_AccessWrapperFactory::defineClass();
    Ext_TestUtil::defineModule();
    Ext_Kernel::defineModule();
    ListModel::defineUtilMethods();
}

void setupGlobalGCMarking()
{
    auto marker = Ext_AnyWrapper::create(QVariant(), [](const QVariant &) {
        ValueReference::markAllReferences();
        ObjectGC::instance()->markNonOwnedObjects();
    });
    rb_gc_register_mark_object(marker);
}

void cleanup()
{
}

void setupEndProc()
{
    rb_set_end_proc([](VALUE) { unprotect(cleanup); }, Qnil);
}

}

extern "C"
void Init_qml()
{
    rb_require("date");
    rb_require("qml/errors");
    rb_require("qml/error_converter");

    unprotect([&] {
        defineMetaTypes();
        defineClasses();
        setupGlobalGCMarking();
        setupEndProc();
    });
}

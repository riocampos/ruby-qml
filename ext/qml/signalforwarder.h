#pragma once
#include "rubyvalue.h"
#include "weakvaluereference.h"
#include "markable.h"
#include <QObject>
#include <QMetaMethod>

namespace RubyQml {

class SignalForwarder : public QObject, public Markable
{
public:
    SignalForwarder(QObject *obj, const QMetaMethod &signal, RubyValue proc);
    ~SignalForwarder();

    int qt_metacall(QMetaObject::Call call, int id, void **args) override;

    void gc_mark() override;

private:
    void forwardArgs(void **args);
    void callProc(const QVariantList &list);

    QMetaMethod mSignal;
    WeakValueReference mProcRef;
};

} // namespace RubyQml


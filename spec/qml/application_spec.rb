require 'spec_helper'

describe QML::Application do
  let(:application) { QML.application }

  describe '.new' do
    it 'fails with QML::ApplicationError' do
      expect { QML::Application.new }.to raise_error(QML::ApplicationError)
    end
  end
  describe '.notify_error' do
    let(:error) do
      begin
        fail "hoge"
      rescue => e
        e
      end
    end

    it 'prints an error to stderr' do
      expect { QML::Application.notify_error(error) }
        .to output(/#{error.message}/).to_stderr
    end
  end
  describe '#engine' do
    it 'returns the default engine of the application' do
      expect(application.engine).to be_a(QML::Engine)
    end
  end
  describe '#context' do
    it 'returns engine.context' do
      expect(application.context).to be(application.engine.context)
    end
  end
  describe '#load_data' do
    let(:data) do
      <<-EOS
        import QtQuick 2.0
        QtObject {
          property string name: 'foo'
        }
      EOS
    end
    it 'loads root object 1from data' do
      application.load_data data
      expect(application.root_component.data).to eq data
      expect(application.root.name).to eq 'foo'
    end
  end
  describe '#load_path' do
    let(:path) { QML::ROOT_PATH + 'spec/assets/testobj.qml' }
    it 'loads root object from path' do
      application.load_path path
      expect(application.root_component.path).to eq path
      expect(application.root.name).to eq 'foo'
    end
  end
end

describe QML do
  describe '.application' do
    it 'returns the QML::Application instance' do
      expect(QML.application).to be_a(QML::Application)
    end
  end
end

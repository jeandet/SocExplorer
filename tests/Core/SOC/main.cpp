#include <QtTest>
#include <QObject>
#include <QString>
#include <SOC/SOC.h>
#include <SOC/socperipheral.h>
#include <memory>
#include <stdint.h>
#include <iostream>


class A_SOC: public QObject
{
    Q_OBJECT
public:
    A_SOC(QObject* parent=Q_NULLPTR)
        :QObject(parent)
    {
        SocExplorerCore::changeLoggerOutput(&std::cerr);
        SOC::addPluginLookupPath(PLUGIN_PATH);
    }
private slots:

    void can_Load_A_Plugin()
    {
        QVERIFY(SOC::loadPlugin("SimplePlugin","SimplePlugin0"));
        auto plugin = SOC::getPlugin("SimplePlugin0");
        QVERIFY(plugin!=Q_NULLPTR);
    }

    void can_Unload_A_Plugin()
    {
        QVERIFY(SOC::loadPlugin("SimplePlugin","SimplePluginToUnload"));
        auto plugin = SOC::getPlugin("SimplePluginToUnload");
        QVERIFY(plugin!=Q_NULLPTR);

        QVERIFY(SOC::unloadPlugin("SimplePluginToUnload"));
        plugin = SOC::getPlugin("SimplePluginToUnload");
        QVERIFY(plugin==Q_NULLPTR);

    }

    void can_Load_A_Plugin_With_Agiven_Instance_Name()
    {
        QVERIFY(SOC::loadPlugin("SimplePlugin", "MyTestPlugin"));
        QVERIFY(SOC::getPlugin( "MyTestPlugin")!=Q_NULLPTR);
    }

    void can_Instanciate_A_Plugin_As_Child()
    {
        QVERIFY(SOC::loadPlugin("SimplePlugin", "Parent"));
        QVERIFY(SOC::loadChildPlugin("SimplePlugin", "Parent","Child"));
        auto child = SOC::getPlugin("Child");
        auto parent = SOC::getPlugin("Parent");
        QVERIFY(child!=Q_NULLPTR);
        QVERIFY(parent!=Q_NULLPTR);
        QVERIFY(child->parentPlugin()==parent.get());
    }

    void calls_A_Callback_On_Plugin_Instanciation()
    {
        bool callbackCalled=false;
        bool foundPlugin;
        const QString pluginInstName{"MyTestPlugin1"};
        auto callback = [&callbackCalled, &foundPlugin, &pluginInstName](const QHash<QString,std::shared_ptr<ISocexplorerPlugin>>& tree)
        {
            callbackCalled = true;
            foundPlugin = tree.contains(pluginInstName);
        };

        auto token = SOC::registerPluginUpdateCallback(callback);
        // Lodaing a plugin should trigger callback and we should find loaded plugin
        QVERIFY(SOC::loadPlugin("SimplePlugin", pluginInstName));
        QVERIFY(SOC::getPlugin( pluginInstName)!=Q_NULLPTR);
        QVERIFY(callbackCalled==true);
        QVERIFY(foundPlugin==true);

        // Unlodaing a plugin should trigger callback and we shouldn't find unloaded plugin
        SOC::unloadPlugin(pluginInstName);
        QVERIFY(SOC::getPlugin( pluginInstName)==Q_NULLPTR);
        QVERIFY(callbackCalled==true);
        QVERIFY(foundPlugin==false);


        SOC::unregisterPluginUpdateCallback(token);
        // Callback is unregistered so it shouldn't be called
        callbackCalled = false;
        foundPlugin = false;
        QVERIFY(SOC::loadPlugin("SimplePlugin", pluginInstName));
        QVERIFY(SOC::getPlugin(pluginInstName)!=Q_NULLPTR);
        QVERIFY(callbackCalled==false);
        QVERIFY(foundPlugin==false);
    }

    void can_Add_A_Peripheral()
    {
        auto pname = QStringLiteral("Peripheral1");
        auto rname = QStringLiteral("Register1");
        auto raddress = address64_t{0x80001000};
        auto p = SOCPeripheral(pname,{{rname,raddress}});
        SOC::addPeripheral(std::move(p));
        QVERIFY(SOC::peripherals().contains(pname));
        QVERIFY(SOC::peripherals()[pname].name()==pname);
        QVERIFY(SOC::peripherals()[pname].registers().size()==1);
        QVERIFY(SOC::peripherals()[pname].registers()[0].name()==rname);
        QVERIFY(SOC::peripherals()[pname].registers()[0].address()==raddress);
    }

};



QTEST_MAIN(A_SOC)
#include "main.moc"

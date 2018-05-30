#include <QtTest>
#include <QObject>
#include <QString>
#include <SOC/SOC.h>
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
        QVERIFY(SOC::loadPlugin("SimplePlugin"));
    }

    void can_Load_A_Plugin_With_Agiven_Instance_Name()
    {
        QVERIFY(SOC::loadPlugin("SimplePlugin", "MyTestPlugin"));
        QVERIFY(SOC::getPlugin( "MyTestPlugin")!=Q_NULLPTR);
    }

    void calls_A_Callback_On_Plugin_Instanciation()
    {
        bool flag=false;
        QString name;
        QString parent;
        auto callback = [&flag, &name, &parent](const QString& nameArg,const QString& parentArg)
        {
            flag = true;
            name = nameArg;
            parent = parentArg;
        };

        auto token = SOC::registerPluginUpdateCallback(callback);

        QVERIFY(SOC::loadPlugin("SimplePlugin", "MyTestPlugin1"));
        QVERIFY(SOC::getPlugin( "MyTestPlugin1")!=Q_NULLPTR);
        QVERIFY(flag==true);
        QVERIFY(parent=="");
        QVERIFY(name=="MyTestPlugin1");

        SOC::unregisterPluginUpdateCallback(token);

        flag = false;
        name="";
        QVERIFY(SOC::loadPlugin("SimplePlugin", "MyTestPlugin"));
        QVERIFY(SOC::getPlugin( "MyTestPlugin")!=Q_NULLPTR);
        QVERIFY(flag==false);
        QVERIFY(parent=="");
        QVERIFY(name=="");
    }
};



QTEST_MAIN(A_SOC)
#include "main.moc"

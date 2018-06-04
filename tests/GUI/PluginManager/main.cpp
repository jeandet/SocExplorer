#include <QtTest>
#include <QObject>
#include <QString>
#include <iostream>

#include <pluginmanagerview.h>
#include <PluginTreeWidget.h>
#include <plugininfowidget.h>
#include <pluginlist.h>
#include <SocExplorerCore.h>
#include <SOC/SOC.h>


class A_PluginManagerGUI: public QObject
{
    Q_OBJECT
public:
    A_PluginManagerGUI(QObject* parent=Q_NULLPTR)
        :QObject(parent)
    {
        SocExplorerCore::changeLoggerOutput(&std::cerr);
        SocExplorerCore::addPluginLookupPath(PLUGIN_PATH);
    }
private slots:

    void lists_Available_Plugins()
    {
        PluginManagerView pluginManager;
        auto loadedPluginsTree = pluginManager.findChild<PluginTreeWidget*>("loadedPluginsTree");
        auto pluginList = pluginManager.findChild<PluginList*>("pluginList");
        auto pluginInfo = pluginManager.findChild<PluginInfoWidget*>("pluginInfo");
        QVERIFY(pluginList->count()>=0);
    }



};



QTEST_MAIN(A_PluginManagerGUI)
#include "main.moc"

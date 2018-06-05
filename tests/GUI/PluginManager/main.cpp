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
#include <unistd.h>


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
        PluginManagerView pluginManager{Q_NULLPTR};
        auto loadedPluginsTree = pluginManager.findChild<PluginTreeWidget*>("loadedPluginsTree");
        auto pluginList = pluginManager.findChild<PluginList*>("pluginList");
        auto pluginInfo = pluginManager.findChild<PluginInfoWidget*>("pluginInfo");
        QVERIFY(pluginList->count() >= 0);
        QVERIFY(pluginList->item(0)->text() == QStringLiteral("SimplePlugin"));
    }

    void loads_A_Plugin_On_Drag_n_Drop()
    {
        PluginManagerView pluginManager{Q_NULLPTR};
        pluginManager.show();
        pluginManager.setFocus();
        auto loadedPluginsTree = pluginManager.findChild<PluginTreeWidget*>("loadedPluginsTree");
        auto pluginList = pluginManager.findChild<PluginList*>("pluginList");
        auto pluginInfo = pluginManager.findChild<PluginInfoWidget*>("pluginInfo");
        QVERIFY(loadedPluginsTree!=Q_NULLPTR);
        QVERIFY(pluginList!=Q_NULLPTR);
        QVERIFY(pluginInfo!=Q_NULLPTR);
        auto item = pluginList->item(0);
        auto itemPos = pluginList->visualItemRect(item).center();
        QTest::qWait(500);
        QTest::mouseMove(pluginList->viewport(),itemPos);
        QTest::qWait(500);
        QTest::mouseClick(pluginList->viewport(), Qt::LeftButton, Qt::NoModifier, itemPos);
        QVERIFY(pluginList->selectedItems()[0]==item);
        QTest::qWait(500);
        QTest::mousePress(pluginList->viewport(), Qt::LeftButton, Qt::NoModifier, itemPos);
        QTest::qWait(500);
        QTest::mouseMove(loadedPluginsTree->viewport());
        QTest::qWait(500);
        QTest::mouseRelease(loadedPluginsTree->viewport(), Qt::LeftButton);
        QTest::qWait(500);
        auto test = loadedPluginsTree->topLevelItem(0);
//        QVERIFY(test != Q_NULLPTR);
//        if(test!=Q_NULLPTR)
//            std::cerr << "hello";
    }



};



QTEST_MAIN(A_PluginManagerGUI)
#include "main.moc"

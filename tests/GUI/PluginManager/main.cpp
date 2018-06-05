#include <QtTest>
#include <QObject>
#include <QString>
#include <QScreen>
#include <QMainWindow>
#include <QDesktopWidget>

#include <iostream>

#include <pluginmanagerview.h>
#include <PluginTreeWidget.h>
#include <plugininfowidget.h>
#include <pluginlist.h>
#include <SocExplorerCore.h>
#include <SOC/SOC.h>
#include <unistd.h>

template< class... >
using void_t = void;

template <class T, class=void>
struct has_viewport : std::false_type{};

template <class T>
struct has_viewport<T, void_t<decltype(std::declval<T>().viewport())>>
       : std::true_type{};

template<typename T>
void mouseMove(T* widget, QPoint pos, Qt::MouseButton mouseModifier)
{
    QCursor::setPos(widget->mapToGlobal(pos));
    QMouseEvent event(QEvent::MouseMove, pos, Qt::NoButton, mouseModifier, Qt::NoModifier);
    if constexpr(has_viewport<T>::value)
        qApp->sendEvent(widget->viewport(), &event);
    else
        qApp->sendEvent(widget, &event);
    qApp->processEvents();
}


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
        QMainWindow mw;
        PluginManagerView* pluginManager=new PluginManagerView(Q_NULLPTR);
        auto loadedPluginsTree = pluginManager->findChild<PluginTreeWidget*>("loadedPluginsTree");
        auto pluginList = pluginManager->findChild<PluginList*>("pluginList");
        auto pluginInfo = pluginManager->findChild<PluginInfoWidget*>("pluginInfo");
        QVERIFY(loadedPluginsTree!=Q_NULLPTR);
        QVERIFY(pluginList!=Q_NULLPTR);
        QVERIFY(pluginInfo!=Q_NULLPTR);
        mw.setCentralWidget(pluginManager);
        mw.setGeometry(QRect(QPoint(QApplication::desktop()->geometry().center() - QPoint(250, 250)),
                                    QSize(500, 500)));
        mw.show();
        pluginList->viewport()->setMouseTracking(true);
        loadedPluginsTree->viewport()->setMouseTracking(true);
        qApp->setActiveWindow(&mw);
        QVERIFY(QTest::qWaitForWindowActive(&mw));
        auto item = pluginList->item(0);
        auto itemCenterPos = pluginList->visualItemRect(item).center();
        QTest::mouseClick(pluginList->viewport(), Qt::LeftButton, Qt::NoModifier, itemCenterPos);
        QVERIFY(pluginList->selectedItems()[0]==item);
        QTest::mousePress(pluginList->viewport(), Qt::LeftButton, Qt::NoModifier, itemCenterPos);
        mouseMove(pluginList,itemCenterPos,Qt::LeftButton);
        itemCenterPos+=QPoint(0,-10);
        QTimer::singleShot(100,[loadedPluginsTree](){
            mouseMove(loadedPluginsTree,loadedPluginsTree->rect().center(),Qt::LeftButton);
            mouseMove(loadedPluginsTree,loadedPluginsTree->rect().center()+QPoint(0,-10),Qt::LeftButton);
            QTest::mouseRelease(loadedPluginsTree->viewport(), Qt::LeftButton);
        });
        mouseMove(pluginList,itemCenterPos,Qt::LeftButton);
        QTest::mouseClick(loadedPluginsTree->viewport(), Qt::LeftButton, Qt::NoModifier, loadedPluginsTree->rect().center());
        QVERIFY(loadedPluginsTree->topLevelItem(0) != Q_NULLPTR);
    }



};



QTEST_MAIN(A_PluginManagerGUI)
#include "main.moc"

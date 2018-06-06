#include <QtTest>
#include <QObject>
#include <QString>
#include <QScreen>
#include <QMainWindow>
#include <QDesktopWidget>

#include <iostream>

#include <plugininfowidget.h>
#include <PluginTreeWidget.h>
#include <pluginmanagerview.h>
#include <pluginlist.h>
#include <SocExplorerCore.h>
#include <SOC/SOC.h>
#include <unistd.h>
#include <cpp_utils.h>

HAS_METHOD(viewport)

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

template <typename T>
void setMouseTracking(T* widget)
{
    if constexpr(has_viewport<T>::value)
    {
        widget->viewport()->setMouseTracking(true);
    }
    else
    {
        widget->setMouseTracking(true);
    }
}

#define SELECT_ITEM(widget, itemIndex, item)\
    auto item = widget->item(itemIndex);\
{\
    auto itemCenterPos = widget->visualItemRect(item).center();\
    QTest::mouseClick(widget->viewport(), Qt::LeftButton, Qt::NoModifier, itemCenterPos);\
    QVERIFY(widget->selectedItems().size() > 0);\
    QVERIFY(widget->selectedItems().contains(item));\
    }


#define GET_CHILD_WIDGET_FOR_GUI_TESTS(parent, child, childType, childName)\
    childType* child = parent.findChild<childType*>(childName); \
    QVERIFY(child!=Q_NULLPTR); \
    setMouseTracking(child);

template<typename T1, typename T2, typename T3, typename T4=void>
void dragnDropItem(T1* sourceWidget, T2* destWidget, T3* item, T4* destItem=Q_NULLPTR)
{
    auto itemCenterPos = sourceWidget->visualItemRect(item).center();
    if constexpr(has_viewport<T1>::value)
    {
        QTest::mousePress(sourceWidget->viewport(), Qt::LeftButton, Qt::NoModifier, itemCenterPos);
    }
    else
    {
        QTest::mousePress(sourceWidget, Qt::LeftButton, Qt::NoModifier, itemCenterPos);
    }
    mouseMove(sourceWidget,itemCenterPos, Qt::LeftButton);
    itemCenterPos+=QPoint(0,-10);
    QTimer::singleShot(100,[destWidget,destItem](){
        mouseMove(destWidget, destWidget->rect().center(),Qt::LeftButton);
        mouseMove(destWidget, destWidget->rect().center()+QPoint(0,-10),Qt::LeftButton);
        if constexpr(!std::is_same<void, T4>::value)
        {
            auto destItemCenterPos = destWidget->visualItemRect(destItem).center();
            QTest::mouseRelease(destWidget, Qt::LeftButton, Qt::NoModifier, destItemCenterPos);
        }
        else if constexpr(has_viewport<T2>::value)
        {
            QTest::mouseRelease(destWidget->viewport(), Qt::LeftButton);
        }
        else
        {
            QTest::mouseRelease(destWidget, Qt::LeftButton);
        }
        QTest::mouseRelease(destWidget->viewport(), Qt::LeftButton);
    });
    mouseMove(sourceWidget,itemCenterPos,Qt::LeftButton);
}


#define PREPARE_GUI_TEST(main_widget)\
    main_widget.setGeometry(QRect(QPoint(QApplication::desktop()->geometry().center() - QPoint(250, 250)),\
    QSize(500, 500)));\
    main_widget.show();\
    qApp->setActiveWindow(&main_widget);\
    QVERIFY(QTest::qWaitForWindowActive(&main_widget))

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
        SOC::unloadAllPlugins();
        PluginManagerView pluginManager{Q_NULLPTR};

        GET_CHILD_WIDGET_FOR_GUI_TESTS(pluginManager, loadedPluginsTree, PluginTreeWidget, "loadedPluginsTree");
        GET_CHILD_WIDGET_FOR_GUI_TESTS(pluginManager, pluginList, PluginList, "pluginList");
        GET_CHILD_WIDGET_FOR_GUI_TESTS(pluginManager, pluginInfo, PluginInfoWidget, "pluginInfo");

        PREPARE_GUI_TEST(pluginManager);

        SELECT_ITEM(pluginList,0,item);

        dragnDropItem(pluginList, loadedPluginsTree, item);

        QTest::mouseClick(loadedPluginsTree->viewport(), Qt::LeftButton, Qt::NoModifier, loadedPluginsTree->rect().center());
        QVERIFY(loadedPluginsTree->topLevelItem(0) != Q_NULLPTR);

    }

    void loads_A_Child_Plugin_On_Drag_n_Drop()
    {
        SOC::unloadAllPlugins();

        PluginManagerView pluginManager{Q_NULLPTR};

        GET_CHILD_WIDGET_FOR_GUI_TESTS(pluginManager, loadedPluginsTree, PluginTreeWidget, "loadedPluginsTree");
        GET_CHILD_WIDGET_FOR_GUI_TESTS(pluginManager, pluginList, PluginList, "pluginList");
        GET_CHILD_WIDGET_FOR_GUI_TESTS(pluginManager, pluginInfo, PluginInfoWidget, "pluginInfo");

        PREPARE_GUI_TEST(pluginManager);

        SELECT_ITEM(pluginList,0,item);

        dragnDropItem(pluginList, loadedPluginsTree, item);

        QTest::mouseClick(loadedPluginsTree->viewport(), Qt::LeftButton, Qt::NoModifier, loadedPluginsTree->rect().center());

        auto rootPluginItem = loadedPluginsTree->topLevelItem(0);

        QVERIFY(rootPluginItem!=Q_NULLPTR);

        dragnDropItem(pluginList, loadedPluginsTree, item, rootPluginItem);

        QVERIFY(SOC::plugins().count()==2);

    }


};



QTEST_MAIN(A_PluginManagerGUI)
#include "main.moc"

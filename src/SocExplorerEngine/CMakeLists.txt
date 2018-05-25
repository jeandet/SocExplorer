SET (engine_SRCS

    ./SOC/socclk.h
    ./SOC/socmodel.h
    ./SOC/registerdata.h
    ./SOC/socexplorerenumdevice.h
    ./PeripheralWidget/src/collapsableperipheralwidget.h
    ./PeripheralWidget/src/peripheralwidget.h
    ./PeripheralWidget/src/socregsviewer.h
    ./PeripheralWidget/src/socregsviewernew.h
    ./PeripheralWidget/src/registerwidget.h
    ./PySocExplorerEngine.h
    ./engine/socexplorerxmlfile.h
    ./engine/socexplorersettings.h
    ./engine/socexplorerengine.h
    ./engine/socexplorerconfigkeys.h
    ./engine/socexplorergui.h
    ./engine/socexplorersettingsdialog.h
    ./engine/socexplorercoresettingsgui.h
    ./pythonQtOut/generated_cpp/PySocExplorerEngine/PySocExplorerEngine0.h
    ./pluginManagerWdgt/plugininfoswdgt.h
    ./pluginManagerWdgt/pluginlist.h
    ./pluginManagerWdgt/pluginmanagerWDGT.h
    ./pluginManagerWdgt/PluginTreeWidget.h
    ./pluginManagerWdgt/pluginmangerview.h
    ./proxy/socexplorerproxy.h
    ./XmlEngine/XMLmodel.h
    ./XmlEngine/xmldriver.h
    ./XmlEngine/XMLdata.h
    ./pluginsInterface/isocexplorerplugin.h
    ./pluginloader/unix/unixpluginloader.h
    ./pluginloader/pluginscache.h
    ./pluginloader/pluginloader.h
    ./memtester/memtester.h
    ./plugins/socexplorerplugin.h
    ./pluginloaderV2/pluginmanager.h


    ./SOC/socmodel.cpp
    ./SOC/socexplorerenumdevice.cpp
    ./SOC/socclk.cpp
    ./SOC/registerdata.cpp
    ./PeripheralWidget/src/socregsviewernew.cpp
    ./PeripheralWidget/src/peripheralwidget.cpp
    ./PeripheralWidget/src/socregsviewer.cpp
    ./PeripheralWidget/src/registerwidget.cpp
    ./PeripheralWidget/src/collapsableperipheralwidget.cpp
    ./engine/socexplorerxmlfile.cpp
    ./engine/socexplorergui.cpp
    ./engine/socexplorerengine.cpp
    ./engine/socexplorersettingsdialog.cpp
    ./engine/socexplorersettings.cpp
    ./engine/socexplorercoresettingsgui.cpp
    ./pythonQtOut/generated_cpp/PySocExplorerEngine/PySocExplorerEngine0.cpp
    ./pythonQtOut/generated_cpp/PySocExplorerEngine/PySocExplorerEngine_init.cpp
    ./pluginManagerWdgt/PluginTreeWidget.cpp
    ./pluginManagerWdgt/plugininfoswdgt.cpp
    ./pluginManagerWdgt/pluginlist.cpp
    ./pluginManagerWdgt/pluginmanagerWDGT.cpp
    ./pluginManagerWdgt/pluginmangerview.cpp
    ./proxy/socexplorerproxy.cpp
    ./XmlEngine/XMLmodel.cpp
    ./XmlEngine/XMLdata.cpp
    ./XmlEngine/xmldriver.cpp
    ./pluginloader/unix/unixpluginloader.cpp
    ./pluginloader/pluginloader.cpp
    ./pluginloader/pluginscache.cpp
    ./memtester/memtester.cpp
    ./plugins/socexplorerplugin.cpp

    ./PeripheralWidget/ressources/peripheralwidget.qrc

    )

QT5_WRAP_UI(
    UiGenerated_SRCS
    ./PeripheralWidget/src/socregsviewernew.ui
    ./PeripheralWidget/src/collapsableperipheralwidget.ui
    ./engine/socexplorersettingsdialog.ui
    ./engine/socexplorercoresettingsgui.ui
    ./pluginManagerWdgt/pluginmangerview.ui
)

add_library(socexplorerengine ${engine_SRCS} ${UiGenerated_SRCS})
SET_TARGET_PROPERTIES(socexplorerengine PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

target_include_directories(socexplorerengine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/plugins>
    $<INSTALL_INTERFACE:include/SocExplorer/SocExplorerEngine/plugins>
)

target_include_directories(socexplorerengine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
    $<INSTALL_INTERFACE:include/SocExplorer>
)

target_include_directories(socexplorerengine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/PeripheralWidget/src>
    $<INSTALL_INTERFACE:include/SocExplorer/SocExplorerEngine/PeripheralWidget>
)

target_include_directories(socexplorerengine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/SOC>
    $<INSTALL_INTERFACE:include/SocExplorer/SocExplorerEngine/SOC>
)

target_include_directories(socexplorerengine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/engine>
    $<INSTALL_INTERFACE:include/SocExplorer/SocExplorerEngine/engine>
)

target_include_directories(socexplorerengine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/XmlEngine>
    $<INSTALL_INTERFACE:include/SocExplorer/SocExplorerEngine/XmlEngine>
)

target_include_directories(socexplorerengine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/memtester>
    $<INSTALL_INTERFACE:include/SocExplorer/SocExplorerEngine/memtester>
)

target_include_directories(socexplorerengine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/pluginloader>
    $<INSTALL_INTERFACE:include/SocExplorer/SocExplorerEngine/pluginloader>
)

target_include_directories(socexplorerengine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/pluginsInterface>
    $<INSTALL_INTERFACE:include/SocExplorer/SocExplorerEngine/pluginsInterface>
)

target_include_directories(socexplorerengine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/pluginManagerWdgt>
    $<INSTALL_INTERFACE:include/SocExplorer/SocExplorerEngine/pluginManagerWdgt>
)

target_include_directories(socexplorerengine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/proxy>
    $<INSTALL_INTERFACE:include/SocExplorer/SocExplorerEngine/proxy>
)

target_include_directories(socexplorerengine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/pluginloaderV2>
    $<INSTALL_INTERFACE:include/SocExplorer/SocExplorerEngine/pluginloaderV2>
)


target_link_libraries(socexplorerengine PUBLIC
  socexplorercommon
  Qt5::Xml
)

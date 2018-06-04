SET (PluginManager_SRCS

    ./PluginTreeWidget.h
    ./pluginmanagerview.h
    ./plugininfowidget.h
    ./pluginlist.h


    ./pluginmanagerview.cpp
    ./plugininfowidget.cpp
    ./PluginTreeWidget.cpp
    ./pluginlist.cpp
    )

QT5_WRAP_UI(UiGenerated_SRCS ./pluginmanagerview.ui ./plugininfowidget.ui)

add_library(pluginmanager ${PluginManager_SRCS} ${UiGenerated_SRCS})
SET_TARGET_PROPERTIES(pluginmanager PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

target_include_directories(pluginmanager PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include/SocExplorer/GUI/PluginManager>
)


target_link_libraries(pluginmanager PUBLIC
  socexplorercore
  socexplorerwidgets
  Qt5::Core
  Qt5::Widgets
  Qt5::Svg
)

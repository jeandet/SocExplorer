add_subdirectory(common)
add_subdirectory(SocExplorerEngine)
add_subdirectory(Core)

SET (SRCS
    ./mainwindowsmenubar.cpp
    ./toolbarcontainer.cpp
    ./main.cpp
    ./sessionmanagerdialog.cpp
    ./dockablepluginmanager.cpp
    ./toolbar.cpp
    ./aboutsocexplorer.cpp
    ./mainwindow.cpp
    ./regsExplorer/regsexplorer.cpp
    ./NicePyConsole/PythonCompleter.cpp
    ./NicePyConsole/NicePyConsole.cpp
    ./NicePyConsole/SimpleConsole.cpp
    ./NicePyConsole/PygmentsHighlighter.cpp
    ./NicePyConsole/PythonCompleterPopup.cpp
    ./PyWdgt/pythonqtscriptingconsoledandd.cpp
    ./PyWdgt/pythonconsole.cpp

    ./sessionmanagerdialog.h
    ./dockablepluginmanager.h
    ./toolbarcontainer.h
    ./toolbar.h
    ./mainwindowsmenubar.h
    ./mainwindow.h
    ./aboutsocexplorer.h
    ./regsExplorer/regsexplorer.h
    ./NicePyConsole/PythonCompleterPopup.h
    ./NicePyConsole/SimpleConsole.h
    ./NicePyConsole/PygmentsHighlighter.h
    ./NicePyConsole/NicePyConsole.h
    ./NicePyConsole/PythonCompleter.h
    ./PyWdgt/pythonqtscriptingconsoledandd.h
    ./PyWdgt/pythonconsole.h

    ../ressources/SocExplorer.qrc
        )

QT5_WRAP_UI(UiGenerated_SRCS
    ./sessionmanagerdialog.ui
    ./regsExplorer/regsexplorernew.ui
)


add_executable(socexplorer ${SRCS} ${UiGenerated_SRCS})
target_link_libraries(socexplorer
  Qt5::Core
  Qt5::Widgets
  Qt5::Network
  Qt5::PrintSupport
  Qt5::Svg
  socexplorerengine
)

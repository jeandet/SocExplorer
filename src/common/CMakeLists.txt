SET (common_SRCS
    ./qipdialogbox.h
    ./gresbstatusquery.h
    ./lppserial/src/RS232.h
    ./rmapoperations.h
    ./qsvgicon.h
    ./SocExplorerPlot.h
    ./memsizewdgt.h
    ./pythonQtOut/generated_cpp/PySocExplorer/PySocExplorer0.h
    ./PySocExplorer.h
    ./tcp_terminal_client.h
    ./qhexspinbox.h
    ./QCustomPlot/qcustomplot.h

    ./memsizewdgt.cpp
    ./pythonQtOut/generated_cpp/PySocExplorer/PySocExplorer0.cpp
    ./pythonQtOut/generated_cpp/PySocExplorer/PySocExplorer_init.cpp
    ./tcp_terminal_client.cpp
    ./qsvgicon.cpp
    ./qhexspinbox.cpp
    ./qipdialogbox.cpp
    ./rmapoperations.cpp
    ./QCustomPlot/qcustomplot.cpp
    ./SocExplorerPlot.cpp
        )


    add_library(socexplorercommon ${common_SRCS})
    SET_TARGET_PROPERTIES(socexplorercommon PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

    target_include_directories(socexplorercommon PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include/SocExplorer/common>
    )
    target_include_directories(socexplorercommon PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/QCustomPlot>
        $<INSTALL_INTERFACE:include/SocExplorer/common/QCustomPlot>
    )



find_package(PythonQt CONFIG QUIET)
if (NOT PythonQt_FOUND)
    execute_process(COMMAND git clone https://github.com/jeandet/PythonQt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    execute_process(COMMAND git checkout patched-8 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/PythonQt)
    set(PythonQt_Wrap_QtAll ON CACHE INTERNAL "" FORCE)
    add_subdirectory(PythonQt)

endif()

target_link_libraries(socexplorercommon PUBLIC
  genericbinaryfiles
  Qt5::Core
  Qt5::Network
  Qt5::Widgets
  Qt5::PrintSupport
  Qt5::Svg
  PythonQt
)


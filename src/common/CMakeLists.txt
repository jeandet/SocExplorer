SET (common_SRCS
    ./qipdialogbox.h
    ./gresbstatusquery.h
    ./lppserial/src/RS232.h
    ./rmapoperations.h
    ./qhexedit/qhexedit.h
    ./qhexedit/commands.h
    ./qhexedit/xbytearray.h
    ./qhexedit/qhexedit_p.h
    ./qsvgicon.h
    ./SocExplorerPlot.h
    ./memsizewdgt.h
    ./pythonQtOut/generated_cpp/PySocExplorer/PySocExplorer0.h
    ./PySocExplorer.h
    ./genericBinaryFiles/abstractbinfile.h
    ./genericBinaryFiles/genericbinaryfilewidget.h
    ./genericBinaryFiles/qtablewidgetintitem.h
    ./genericBinaryFiles/filelist.h
    ./genericBinaryFiles/qvpushbutton.h
    ./genericBinaryFiles/elf/elfinfowdgt.h
    ./genericBinaryFiles/elf/elffilewidget.h
    ./genericBinaryFiles/elf/elffile.h
    ./genericBinaryFiles/elf/elfparser.h
    ./genericBinaryFiles/BinFile/binaryfile.h
    ./genericBinaryFiles/BinFile/binaryfilewidget.h
    ./genericBinaryFiles/srec/srecfile.h
    ./genericBinaryFiles/srec/srecfilewidget.h
    ./tcp_terminal_client.h
    ./qhexspinbox.h
    ./QCustomPlot/qcustomplot.h

    ./qhexedit/xbytearray.cpp
    ./qhexedit/qhexedit_p.cpp
    ./qhexedit/qhexedit.cpp
    ./qhexedit/commands.cpp
    ./memsizewdgt.cpp
    ./pythonQtOut/generated_cpp/PySocExplorer/PySocExplorer0.cpp
    ./pythonQtOut/generated_cpp/PySocExplorer/PySocExplorer_init.cpp
    ./tcp_terminal_client.cpp
    ./qsvgicon.cpp
    ./qhexspinbox.cpp
    ./qipdialogbox.cpp
    ./genericBinaryFiles/elf/elfparser.cpp
    ./genericBinaryFiles/elf/elffilewidget.cpp
    ./genericBinaryFiles/elf/elffile.cpp
    ./genericBinaryFiles/elf/elfinfowdgt.cpp
    ./genericBinaryFiles/filelist.cpp
    ./genericBinaryFiles/qvpushbutton.cpp
    ./genericBinaryFiles/BinFile/binaryfilewidget.cpp
    ./genericBinaryFiles/BinFile/binaryfile.cpp
    ./genericBinaryFiles/qtablewidgetintitem.cpp
    ./genericBinaryFiles/genericbinaryfilewidget.cpp
    ./genericBinaryFiles/srec/srecfilewidget.cpp
    ./genericBinaryFiles/srec/srecfile.cpp
    ./genericBinaryFiles/abstractbinfile.cpp
    ./rmapoperations.cpp
    ./QCustomPlot/qcustomplot.cpp
    ./SocExplorerPlot.cpp

    ./genericBinaryFiles/ressources/genericBinaryFiles.qrc
        )

    QT5_WRAP_UI(
        UiGenerated_SRCS
        ./genericBinaryFiles/elf/elffilewidget.ui
        ./genericBinaryFiles/BinFile/binaryfilewidget.ui
        ./genericBinaryFiles/genericbinaryfilewidget.ui
        ./genericBinaryFiles/srec/srecfilewidget.ui
    )


    add_library(socexplorercommon ${common_SRCS} ${UiGenerated_SRCS})
    SET_TARGET_PROPERTIES(socexplorercommon PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

    target_include_directories(socexplorercommon PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include/SocExplorer/common>
    )
    target_include_directories(socexplorercommon PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/QCustomPlot>
        $<INSTALL_INTERFACE:include/SocExplorer/common/QCustomPlot>
    )
    target_include_directories(socexplorercommon PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/genericBinaryFiles>
        $<INSTALL_INTERFACE:include/SocExplorer/common/genericBinaryFiles>
    )
    target_include_directories(socexplorercommon PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/genericBinaryFiles/BinFile>
        $<INSTALL_INTERFACE:include/SocExplorer/common/genericBinaryFiles/BinFile>
    )
    target_include_directories(socexplorercommon PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/genericBinaryFiles/elf>
        $<INSTALL_INTERFACE:include/SocExplorer/common/genericBinaryFiles/elf>
    )
    target_include_directories(socexplorercommon PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/genericBinaryFiles/srec>
        $<INSTALL_INTERFACE:include/SocExplorer/common/genericBinaryFiles/srec>
    )
    target_include_directories(socexplorercommon PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/qhexedit>
        $<INSTALL_INTERFACE:include/SocExplorer/common/qhexedit>
    )



find_package(PythonQt CONFIG QUIET)
if (NOT PythonQt_FOUND)
    execute_process(COMMAND git clone https://github.com/jeandet/PythonQt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    execute_process(COMMAND git checkout patched-8 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/PythonQt)
    set(PythonQt_Wrap_QtAll ON CACHE INTERNAL "" FORCE)
    add_subdirectory(PythonQt)

endif()

include(FindPkgConfig)
pkg_search_module(LIBELF REQUIRED libelf)
target_link_libraries(socexplorercommon PUBLIC
  Qt5::Core
  Qt5::Network
  Qt5::Widgets
  Qt5::PrintSupport
  Qt5::Svg
  PythonQt
  ${LIBELF_LIBRARIES}
)


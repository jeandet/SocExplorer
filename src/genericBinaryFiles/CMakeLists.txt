SET (genericBinFiles_SRCS
    abstractbinfile.h
    genericbinaryfilewidget.h
    qtablewidgetintitem.h
    filelist.h
    qvpushbutton.h
    elf/elfinfowdgt.h
    elf/elffilewidget.h
    elf/elffile.h
    elf/elfparser.h
    BinFile/binaryfile.h
    BinFile/binaryfilewidget.h
    srec/srecfile.h
    srec/srecfilewidget.h

    elf/elfparser.cpp
    elf/elffilewidget.cpp
    elf/elffile.cpp
    elf/elfinfowdgt.cpp
    filelist.cpp
    qvpushbutton.cpp
    BinFile/binaryfilewidget.cpp
    BinFile/binaryfile.cpp
    qtablewidgetintitem.cpp
    genericbinaryfilewidget.cpp
    srec/srecfilewidget.cpp
    srec/srecfile.cpp
    abstractbinfile.cpp

    ressources/genericBinaryFiles.qrc
        )

    QT5_WRAP_UI(
        UiGenerated_SRCS
        elf/elffilewidget.ui
        BinFile/binaryfilewidget.ui
        genericbinaryfilewidget.ui
        srec/srecfilewidget.ui
    )


    add_library(genericbinaryfiles ${genericBinFiles_SRCS} ${UiGenerated_SRCS})
    SET_TARGET_PROPERTIES(genericbinaryfiles PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

    target_include_directories(genericbinaryfiles PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include/SocExplorer/genericBinaryFiles>
    )

    target_include_directories(genericbinaryfiles PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/BinFile>
        $<INSTALL_INTERFACE:include/SocExplorer/genericBinaryFiles/BinFile>
    )

    target_include_directories(genericbinaryfiles PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/elf>
        $<INSTALL_INTERFACE:include/SocExplorer/genericBinaryFiles/elf>
    )

    target_include_directories(genericbinaryfiles PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/srec>
        $<INSTALL_INTERFACE:include/SocExplorer/genericBinaryFiles/srec>
    )


include(FindPkgConfig)
pkg_search_module(LIBELF REQUIRED libelf)
target_link_libraries(genericbinaryfiles PUBLIC
  Qt5::Core
  Qt5::Widgets
  qhexedit
  ${LIBELF_LIBRARIES}
)


SET (qhexedit_SRCS
     qhexedit.h
     commands.h
     xbytearray.h
     qhexedit_p.h

     xbytearray.cpp
     qhexedit_p.cpp
     qhexedit.cpp
     commands.cpp
        )



    add_library(qhexedit ${qhexedit_SRCS})
    SET_TARGET_PROPERTIES(qhexedit PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

    target_include_directories(qhexedit PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include/SocExplorer/qhexedit>
    )

target_link_libraries(qhexedit PUBLIC
  Qt5::Core
  Qt5::Widgets
)


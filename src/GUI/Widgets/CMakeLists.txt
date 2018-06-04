SET (Widgets_SRCS
    ./QSvgIcon/qsvgicon.h

    ./QSvgIcon/qsvgicon.cpp
    )

#QT5_WRAP_UI(UiGenerated_SRCS ./file.ui)

add_library(socexplorerwidgets ${Widgets_SRCS})
SET_TARGET_PROPERTIES(socexplorerwidgets PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS TRUE)


target_include_directories(socexplorerwidgets PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
    $<INSTALL_INTERFACE:include/SocExplorer/GUI/Widgets>
)

target_include_directories(socexplorerwidgets PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/QSvgIcon>
    $<INSTALL_INTERFACE:include/SocExplorer/GUI/Widgets/QSvgIcon>
)


target_link_libraries(socexplorerwidgets PUBLIC
  Qt5::Core
  Qt5::Widgets
  Qt5::Svg
)

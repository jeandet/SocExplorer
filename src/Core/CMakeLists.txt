SET (core_SRCS

    ./pluginsInterface/isocexplorerplugin.h
    ./pluginloaderV2/pluginmanager.h
    ./memtester/memtester.h
    ./SOC/SOC.h
    ./Logger/Logger.h
    ./SocExplorerCore.h
    ./address.h

    ./memtester/memtester.cpp
    ./SOC/SOC.cpp
    ./Logger/Logger.cpp
    ./SocExplorerCore.cpp

    )

add_library(socexplorercore ${core_SRCS})
SET_TARGET_PROPERTIES(socexplorercore PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

target_include_directories(socexplorercore PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include/SocExplorer/Core>
)

target_include_directories(socexplorercore PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/pluginsInterface>
    $<INSTALL_INTERFACE:include/SocExplorer/Core/pluginsInterface>
)

target_include_directories(socexplorercore PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Logger>
    $<INSTALL_INTERFACE:include/SocExplorer/Core/Logger>
)

target_include_directories(socexplorercore PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/pluginloaderV2>
    $<INSTALL_INTERFACE:include/SocExplorer/Core/pluginloaderV2>
)

target_include_directories(socexplorercore PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/memtester>
    $<INSTALL_INTERFACE:include/SocExplorer/Core/memtester>
)

target_link_libraries(socexplorercore PUBLIC
  socexplorercommon
  opaque
)

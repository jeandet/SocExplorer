FILE (GLOB_RECURSE opaque_SRCs
	        include/*.hpp
	        )

add_library(opaque ${opaque_SRCs})
target_include_directories(opaque PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/SocExplorer/opaque>

    )
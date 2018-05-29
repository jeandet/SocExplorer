add_library(simple_plugin SHARED simple_plugin.h simple_plugin.cpp)
target_link_libraries(simple_plugin socexplorerengine)

add_executable(test_simple_plugin main.cpp)
add_definitions(-DPLUGIN_PATH="${CMAKE_CURRENT_BINARY_DIR}")
target_link_libraries(test_simple_plugin socexplorercore Qt5::Test)
add_test(NAME simple_plugin_test COMMAND test_simple_plugin)

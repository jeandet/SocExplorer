add_executable(test_plugin_mabager_gui main.cpp)
target_link_libraries(test_plugin_mabager_gui pluginmanager Qt5::Test)
add_definitions(-DPLUGIN_PATH="${CMAKE_CURRENT_BINARY_DIR}/../../Plugins/simple_plugin")
add_test(NAME plugin_mabager_gui_test COMMAND test_plugin_mabager_gui ${TestArgs})

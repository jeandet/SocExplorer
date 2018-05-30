add_executable(test_SOC main.cpp)
add_definitions(-DPLUGIN_PATH="${CMAKE_CURRENT_BINARY_DIR}/../../Plugins/simple_plugin")
target_link_libraries(test_SOC socexplorercore Qt5::Test)
add_test(NAME SOC_test COMMAND test_SOC ${TestArgs})

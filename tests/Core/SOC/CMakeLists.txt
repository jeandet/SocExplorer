add_executable(test_SOC main.cpp)

target_link_libraries(test_SOC socexplorercore Qt5::Test)
add_test(NAME SOC_test COMMAND test_SOC)

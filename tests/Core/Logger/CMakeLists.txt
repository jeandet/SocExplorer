add_executable(test_Logger main.cpp)

target_link_libraries(test_Logger socexplorercore Qt5::Test)
add_test(NAME Logger_test COMMAND test_Logger)

add_executable(test_bytes_utils main.cpp)
target_link_libraries(test_bytes_utils socexplorercore Qt5::Test)
add_test(NAME bytes_utils_test COMMAND test_bytes_utils ${TestArgs})

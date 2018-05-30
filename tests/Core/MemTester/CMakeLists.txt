add_executable(test_MemTester main.cpp)

target_link_libraries(test_MemTester socexplorercore Qt5::Test)
add_test(NAME MemTester_test COMMAND test_MemTester ${TestArgs})

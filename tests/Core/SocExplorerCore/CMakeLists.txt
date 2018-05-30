add_executable(test_SocExplorerCore main.cpp)

target_link_libraries(test_SocExplorerCore socexplorercore Qt5::Test)
add_test(NAME SocExplorerCore_test COMMAND test_SocExplorerCore ${TestArgs})

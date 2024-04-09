cmake_minimum_required (VERSION 3.12)

# Every test is an executable, that return 0 from main if test passed and
# non-zero value otherwise

# For test we need another executable
add_executable (mytest my_test.cpp)
# Link your player library to test
target_link_libraries (mytest tttplayer)
# Add headers from tttplayer and from tttlib
target_include_directories (mytest PUBLIC .. ../../tttlib)
# Do not forget to add CXX20
set_target_properties (mytest PROPERTIES CXX_STANDARD 20)

# Add command for test execution, in our case just `mytest` itself
add_test (NAME "my_test_for_player" COMMAND mytest)

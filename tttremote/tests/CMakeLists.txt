cmake_minimum_required (VERSION 3.12)

# Every test is an executable, that return 0 from main if test passed and
# non-zero value otherwise

# For test we need another executable
add_executable (test_encoder test_encoder.cpp ../encoder.cpp)
# Link your player library to test
target_link_libraries (test_encoder tttlib)
# Add headers from tttplayer and from tttlib
target_include_directories (test_encoder PUBLIC .. ../../tttlib)
# Do not forget to add CXX20
set_target_properties (test_encoder PROPERTIES CXX_STANDARD 20)

# Add command for test execution, in our case just `mytest` itself
add_test (NAME "test_for_encoders" COMMAND test_encoder)
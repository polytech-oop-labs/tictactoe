cmake_minimum_required (VERSION 3.12)

add_executable (test_zmq_driver test_zmq_driver.cpp)
target_link_libraries (test_zmq_driver tttremote)
target_include_directories (test_zmq_driver PUBLIC . .. ../../tttlib)

add_executable (test_zmq_sender test_zmq_request_sender.cpp)
target_link_libraries (test_zmq_sender tttremote)
target_include_directories (test_zmq_sender PUBLIC . .. ../../tttlib)

set_target_properties (test_zmq_driver test_zmq_sender PROPERTIES CXX_STANDARD 20)

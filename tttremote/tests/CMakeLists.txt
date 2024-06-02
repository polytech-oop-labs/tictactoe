cmake_minimum_required (VERSION 3.12)

add_executable (test_zmq_driver test_zmq_driver.cpp)
target_link_libraries (test_zmq_driver tttremote)
target_include_directories (test_zmq_driver PUBLIC . .. ../../tttlib)

add_executable (test_zmq_sender test_zmq_request_sender.cpp)
target_link_libraries (test_zmq_sender tttremote)
target_include_directories (test_zmq_sender PUBLIC . .. ../../tttlib)

add_executable (test_message_coding test_message_coding.cpp)
target_link_libraries (test_message_coding tttremote)
target_include_directories (test_message_coding PUBLIC . .. ../../tttlib)

add_test(NAME "test_message_encoder" COMMAND test_message_coding)

set_target_properties (test_zmq_driver test_zmq_sender test_message_coding 
    PROPERTIES CXX_STANDARD 20)

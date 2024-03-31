cmake_minimum_required (VERSION 3.12)

add_executable(test_events test_events.cpp)
target_link_libraries(test_events tttlib)
target_include_directories(test_events PRIVATE ..)

add_test (NAME "test_events_move_event" COMMAND test_events move_event)
add_test (NAME "test_events_message_event" COMMAND test_events message_event)
add_test (NAME "test_events_bad_cast" COMMAND test_events bad_cast)

add_executable(test_fields test_fields.cpp)
target_link_libraries(test_fields tttlib)
target_include_directories(test_fields PRIVATE ..)

add_test (NAME "test_fields_fixed_size" COMMAND test_fields fixed_size_field)
add_test (NAME "test_fields_fixed_size_iterator" COMMAND test_fields fixed_size_iterator)
add_test (NAME "test_fields_infinite_field" COMMAND test_fields infinite_field)

add_executable(test_engine test_engine.cpp)
target_link_libraries(test_engine tttlib)
target_include_directories(test_engine PRIVATE ..)

add_test (NAME "test_engine_basic_events" COMMAND test_engine basic_events)
add_test (NAME "test_engine_winning_moves" COMMAND test_engine winning_moves)
add_test (NAME "test_engine_draw" COMMAND test_engine draw)

set_target_properties (test_events test_fields test_engine  PROPERTIES CXX_STANDARD 20)
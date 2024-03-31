cmake_minimum_required (VERSION 3.12)

set (TTTLIB_SOURCES events.cpp basic_fields.cpp composed_observer.cpp game_engine.cpp)
set (TTTLIB_HEADERS .)

add_library (tttlib STATIC ${TTTLIB_SOURCES})
target_include_directories (tttlib PRIVATE ${TTTLIB_HEADERS})
 
set_target_properties (tttlib PROPERTIES CXX_STANDARD 20)

add_subdirectory ("tests")

cmake_minimum_required (VERSION 3.12)

find_library (ZMQ_LIB libzmq.a libzmq-mt-4_3_6.lib PATHS extern/build)

if (NOT ZMQ_LIB)
    message (FATAL_ERROR 
        "Cannot find libzmq. Please, follow instructions in 'extern/README.md'"
        "to install it.")
endif ()

find_path (ZMQLIB_HEADER zmq.h PATHS ./extern/libzmq/include)
find_path (CPPZMQ_HEADER zmq.hpp PATHS ./extern/cppzmq)

if (NOT ZMQLIB_HEADER OR NOT CPPZMQ_HEADER)
    message (FATAL_ERROR 
        "Cannot find libzmq or cppzmq headers. "
        "Make sure that you have imported git submodules for them. "
        "Please, refer to 'extern/README.md' for instructions for that.")
endif()

set (ZMQ_HEADERS ${ZMQLIB_HEADER} ${CPPZMQ_HEADER})

add_library (tttremote STATIC messaging_driver.cpp zmq_drivers.cpp messages.cpp)
target_include_directories (tttremote PUBLIC . ../tttlib ${ZMQ_HEADERS})
target_link_libraries (tttremote PUBLIC tttlib ${ZMQ_LIB})

set_target_properties (tttremote PROPERTIES CXX_STANDARD 20)

add_subdirectory("tests")

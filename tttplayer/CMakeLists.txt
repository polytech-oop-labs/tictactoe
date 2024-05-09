cmake_minimum_required (VERSION 3.12)

# Here you can add all the source files for player
set (PLAYER_SOURCES my_player.cpp ncurses_observer.cpp)

find_package(Curses REQUIRED)

# Create a library for your player
add_library (tttplayer STATIC ${PLAYER_SOURCES})
# Link game library to your player
target_link_libraries (tttplayer tttlib ${CURSES_LIBRARIES})
# Add headers from `tttlib` to your library
target_include_directories (tttplayer PUBLIC ../tttlib . ${CURSES_INCLUDE_DIR})

# You must set this standard to compile, because tttlib some of headers uses
# CXX20 features
set_target_properties (tttplayer PROPERTIES CXX_STANDARD 20)

# Add executable for debugging
add_executable (main main.cpp)
# Link your player to this executable
target_link_libraries (main tttplayer)
# Add headers for this executable
target_include_directories (main PRIVATE . ../tttlib)

# Also set CXX standard
set_target_properties (main PROPERTIES CXX_STANDARD 20)


# Add CMakeLists.txt from `tests` subfolder
add_subdirectory ("tests")

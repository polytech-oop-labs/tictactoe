cmake_minimum_required (VERSION 3.12)

project ("TicTacToe")

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
if (CMAKE_EXPORT_COMPILE_COMMANDS)
  set (CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES 
      ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif()

enable_testing ()
add_subdirectory ("tttlib")
add_subdirectory ("tttplayer")


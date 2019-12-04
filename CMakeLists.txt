cmake_minimum_required(VERSION 2.8)
project(AStar)

add_subdirectory(astar_lib)
add_subdirectory(astar_win)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT AStarWin)


project(profiling)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O2 -std=c++11 -Wall -Werror -pedantic -pedantic-errors")

add_executable(game
    main.cpp
    Game.cpp
)

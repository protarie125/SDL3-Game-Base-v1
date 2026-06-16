cmake_minimum_required(VERSION 3.28)
project(sdl3_game_base CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_STANDARD_REQUIRED ON)

find_package(SDL3 REQUIRED)

add_executable(sdl3_game_base src/main.cpp)
target_link_libraries(sdl3_game_base PRIVATE SDL3::SDL3)


cmake_minimum_required(VERSION 3.16)
project(MiniFloorplan)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(minifloorplan
    src/main.cpp
    src/Options.cpp
)
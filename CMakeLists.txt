cmake_minimum_required(VERSION 3.25)
project(TestSystems)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES
        src/*.h
        src/*.hpp
        src/*.cpp
        Include/*.h
        Include/*.hpp
        Include/*.cpp
        )

add_executable(TestSystems main.cpp ${SOURCES})

include_directories(Include)

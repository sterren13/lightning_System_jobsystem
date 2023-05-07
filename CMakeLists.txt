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

add_executable(TestSystems main.cpp ${SOURCES} Include/TaskSystem/ITask.h Include/TaskSystem/Scheduler.cpp Include/TaskSystem/Scheduler.h Include/TaskSystem/TaskHandel.cpp Include/TaskSystem/TaskHandel.h Include/TaskSystem/TaskQueue.cpp Include/TaskSystem/TaskQueue.h Include/TaskSystem/Worker.cpp Include/TaskSystem/Worker.h)

include_directories(Include)

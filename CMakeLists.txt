cmake_minimum_required(VERSION 3.1)
project(btrForensics)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 11)

#set(CMAKE_CXX_FLAGS "-Wall -ltsk")

include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(Basics)
add_subdirectory(Utility)
add_subdirectory(Trees)
add_subdirectory(Tools)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

add_executable(btrfrsc btrfrsc.cpp)

target_link_libraries(btrfrsc Tools Trees)
target_link_libraries(btrfrsc Trees Tools Basics Utility tsk)

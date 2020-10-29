cmake_minimum_required(VERSION 3.18)
project(Application)

set(CMAKE_CXX_STANDARD 17)

add_executable(Application main.cpp NDB.h NDB.cpp NDBGenerator.h NDBGenerator.cpp)
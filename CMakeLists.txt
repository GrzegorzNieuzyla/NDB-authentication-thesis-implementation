cmake_minimum_required(VERSION 3.18)
project(Application)

set(CMAKE_CXX_STANDARD 17)

add_executable(Application main.cpp NDB.h NDB.cpp Generators/NDBPrefixGenerator.h Generators/NDBPrefixGenerator.cpp Generators/NDBGenerator.h VectorView.h Generators/NDBGenerator.cpp Validator/NDBValidator.cpp Validator/NDBValidator.h DB.h DB.cpp)
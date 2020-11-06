cmake_minimum_required(VERSION 3.18)
project(Application)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast")
add_executable(Application main.cpp NDB.h NDB.cpp Generators/NDBPrefixGenerator.h Generators/NDBPrefixGenerator.cpp Generators/NDBRandomizedGenerator.cpp Generators/NDBRandomizedGenerator.h Generators/NDBGenerator.h VectorView.h Generators/NDBGenerator.cpp Validator/NDBValidator.cpp Validator/NDBValidator.h DB.h DB.cpp Utils/RandomValuesGenerator.h Utils/Permutation.h Utils/Benchmark.h)
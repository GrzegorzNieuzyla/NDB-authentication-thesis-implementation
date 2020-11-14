cmake_minimum_required(VERSION 3.0)
project(Application)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast")
add_executable(Application
        main.cpp
        NDB.h NDB.cpp
        Generators/NDBPrefixGenerator.h Generators/NDBPrefixGenerator.cpp
        Generators/NDBRandomizedGenerator.cpp Generators/NDBRandomizedGenerator.h
        Generators/NDBRandomizedGenerator2.cpp Generators/NDBRandomizedGenerator2.h
        Generators/NDBGenerator.h Generators/NDBGenerator.cpp
        VectorView.h
        Validator/NDBValidator.h Validator/NDBValidator.cpp
        DB.h DB.cpp
        Utils/RandomValuesGenerator.h
        Utils/Permutation.h
        Utils/Benchmark.h
        Utils/NDBUtils.h
        Utils/RandomSeedProvider.h
        Streams/Stream.h
        Streams/DimacsFileStream.h Streams/DimacsFileStream.cpp
        Streams/FileStream.h Streams/FileStream.cpp
        Streams/NDBStream.h Streams/NDBStream.cpp)
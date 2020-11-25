cmake_minimum_required(VERSION 3.0)
project(Application)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast")
add_executable(Application
        main.cpp
        NDB.h NDB.cpp
        Generators/NDB_PrefixGenerator.h Generators/NDB_PrefixGenerator.cpp
        Generators/NDB_RandomizedOldGenerator.cpp Generators/NDB_RandomizedOldGenerator.h
        Generators/NDB_RandomizedGenerator.cpp Generators/NDB_RandomizedGenerator.h
        Generators/NDB_Generator.h Generators/NDB_Generator.cpp
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
        Streams/NDBFileStream.h Streams/NDBFileStream.cpp
        Streams/NDBStream.h Streams/NDBStream.cpp
        Generators/NDB_QHiddenGenerator.cpp Generators/NDB_QHiddenGenerator.h
        Generators/NDB_KHiddenGenerator.cpp Generators/NDB_KHiddenGenerator.h Generators/NDB_0HiddenGenerator.cpp Generators/NDB_0HiddenGenerator.h Generators/NDB_HybridGenerator.cpp Generators/NDB_HybridGenerator.h CLIInterface.cpp CLIInterface.h)

target_link_libraries(Application boost_program_options)

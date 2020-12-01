cmake_minimum_required(VERSION 3.0)
project(Application)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast")
find_library(zChaff sat .)
add_executable(NDBGenerator
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
        Generators/NDB_KHiddenGenerator.cpp Generators/NDB_KHiddenGenerator.h
        Generators/NDB_0HiddenGenerator.cpp Generators/NDB_0HiddenGenerator.h
        Generators/NDB_HybridGenerator.cpp Generators/NDB_HybridGenerator.h
        CLIInterface.cpp CLIInterface.h
        Interfaces/ZChaffInterface.cpp Interfaces/ZChaffInterface.h
        Tests/SuperfluousStringTest.cpp Tests/SuperfluousStringTest.h
        Tests/DistributionTest.cpp Tests/DistributionTest.h
        Utils/FileUtils.h
        Tests/SuperfluousStringBatchTest.cpp Tests/SuperfluousStringBatchTest.h
        Utils/Checksum.h
        Interfaces/walksat.c Interfaces/Walksat.h Interfaces/WalksatInterface.cpp Interfaces/WalksatInterface.h Utils/RandomValuesGenerator.cpp Tests/ZChaffSolveTest.cpp Tests/ZChaffSolveTest.h Tests/WalkSatSolveTest.cpp Tests/WalkSatSolveTest.h Tests/PrefixBatchTest.cpp Tests/PrefixBatchTest.h Tests/GeneratorBatchTest.h Tests/RandomizerBatchTest.cpp Tests/RandomizerBatchTest.h Tests/QHiddenBatchTest.cpp Tests/QHiddenBatchTest.h Tests/KHiddenBatchTest.cpp Tests/KHiddenBatchTest.h Tests/HybridBatchTest.cpp Tests/HybridBatchTest.h)

target_link_libraries(NDBGenerator boost_program_options)
target_link_libraries(NDBGenerator ${zChaff})
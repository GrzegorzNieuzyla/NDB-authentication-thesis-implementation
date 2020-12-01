#pragma once

#include <string>
#include "ZChaffSolveTest.h"
#include "WalkSatSolveTest.h"
#include <boost/algorithm/string.hpp>
#include "../Utils/RandomValuesGenerator.h"
#include <fstream>
#include <utility>


class GeneratorBatchTest
{
public:
    GeneratorBatchTest() = delete;
    virtual void Run(const std::string& filename) = 0;
    enum class Solver
    {
        ZChaff,
        WalkSat
    };
    explicit GeneratorBatchTest(Solver solver) : _solver(solver) {}

protected:
    Solver _solver;
};
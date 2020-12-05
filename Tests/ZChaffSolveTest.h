#pragma once
#include <cstddef>
#include "../Generators/NDB_Generator.h"

class ZChaffSolveTest
{
public:
    struct Results
    {
        std::size_t decisions;
        std::size_t genTime;
        std::size_t solveTime;
        bool solved;
    };

    explicit ZChaffSolveTest(NDB_Generator &generator);
    Results Solve();

private:
    NDB_Generator& _generator;
};




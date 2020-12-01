#pragma once


#include "../Generators/NDB_Generator.h"
#include "../Interfaces/Walksat.h"

class WalkSatSolveTest
{
public:
    explicit WalkSatSolveTest(NDB_Generator &generator);
    WalksatResult Solve();
private:
    NDB_Generator& _generator;
};




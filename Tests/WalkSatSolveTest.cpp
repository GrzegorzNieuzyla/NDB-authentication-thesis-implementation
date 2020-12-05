#include "WalkSatSolveTest.h"
#include "../Streams/NDBStream.h"
#include "../Interfaces/WalksatInterface.h"

WalkSatSolveTest::WalkSatSolveTest(NDB_Generator &generator) : _generator(generator)
{}

WalksatResult WalkSatSolveTest::Solve()
{
    NDBStream stream;
    std::clock_t c_start = std::clock();
    _generator.Generate(stream);
    std::clock_t c_end = std::clock();
    auto results = WalksatInterface().Solve(stream.Ndb());
    results.genTime = (c_end-c_start) / (CLOCKS_PER_SEC / 1000);
    return results;
}

#include "ZChaffSolveTest.h"
#include "../Streams/NDBStream.h"
#include "../Interfaces/ZChaffInterface.h"

ZChaffSolveTest::ZChaffSolveTest(NDB_Generator &generator) : _generator(generator)
{

}

ZChaffSolveTest::Results ZChaffSolveTest::Solve()
{
    Results results{};
    NDBStream stream;
    std::clock_t c_start = std::clock();
    _generator.Generate(stream);
    std::clock_t c_end = std::clock();
    results.genTime = (c_end-c_start) / (CLOCKS_PER_SEC / 1000);
    auto solution = ZChaffInterface().Solve(stream.Ndb());
    results.solved = solution.record.has_value();
    results.decisions = solution.decisions;
    results.solveTime = solution.seconds;
    return results;
}

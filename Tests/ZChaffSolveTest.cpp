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
    _generator.Generate(stream);
    auto solution = ZChaffInterface().Solve(stream.Ndb());
    results.solved = solution.record.has_value();
    results.decisions = solution.decisions;
    results.seconds = solution.seconds;
    return results;
}

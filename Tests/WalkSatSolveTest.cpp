#include "WalkSatSolveTest.h"
#include "../Streams/NDBStream.h"
#include "../Interfaces/WalksatInterface.h"

WalkSatSolveTest::WalkSatSolveTest(NDB_Generator &generator) : _generator(generator)
{}

WalksatResult WalkSatSolveTest::Solve()
{
    NDBStream stream;
    _generator.Generate(stream);
    return WalksatInterface().Solve(stream.Ndb());
}

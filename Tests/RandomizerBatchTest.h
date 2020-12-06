#pragma once


#include "GeneratorBatchTest.h"

class RandomizerBatchTest : public GeneratorBatchTest
{
public:
    void Run(const std::string &filename) override;
    RandomizerBatchTest(Solver solver);

private:
    void RunTest(std::ofstream& file, int count, int length);
};




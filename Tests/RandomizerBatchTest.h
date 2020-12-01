#pragma once


#include "GeneratorBatchTest.h"

class RandomizerBatchTest : public GeneratorBatchTest
{
public:
    void Run(const std::string &filename) override;
    RandomizerBatchTest(Solver solver, std::vector<int> counts, std::vector<int> lengths, int repeat = 1);

private:
    std::vector<int> _lengths;
    std::vector<int> _counts;
    int _repeat;

    void RunTest(std::ofstream& file, int count, int length);
};




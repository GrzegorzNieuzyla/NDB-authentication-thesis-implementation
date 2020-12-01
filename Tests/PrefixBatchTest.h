#pragma once


#include <vector>
#include <string>
#include "GeneratorBatchTest.h"

class PrefixBatchTest : public GeneratorBatchTest
{
public:
    PrefixBatchTest(Solver solver, std::vector<int> counts, std::vector<int> lengths, int repeat = 1);
    void Run(const std::string& filename) override;
private:
    void RunTest(std::ofstream& file, int count, int length);
    std::vector<int> _counts;
    std::vector<int> _lengths;
    int _repeat;
};




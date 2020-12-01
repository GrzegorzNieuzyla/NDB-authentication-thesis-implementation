#pragma once

#include "GeneratorBatchTest.h"

class HybridBatchTest : public GeneratorBatchTest
{
public:
    HybridBatchTest(Solver solver, std::vector<int> lengths, std::vector<double> probabilityRatios,
                    std::vector<double> recordCountRatios, int repeat);

    void Run(const std::string &filename) override;

private:
    std::vector<int> _lengths;
    std::vector<double> _probabilityRatios;
    std::vector<double> _recordCountRatios;

    int _repeat;

    void RunTest(std::ofstream &file, int length, double ratio, double count);
};




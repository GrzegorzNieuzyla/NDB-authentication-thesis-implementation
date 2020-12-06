#pragma once
#include "GeneratorBatchTest.h"

class KHiddenBatchTest : public GeneratorBatchTest
{
public:
    KHiddenBatchTest(Solver solver);
    void Run(const std::string &filename) override;

private:
    void RunTest(std::ofstream &file, int length, const std::vector<double> &prob, double count, int bits);
};




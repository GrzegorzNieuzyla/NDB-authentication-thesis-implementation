#pragma once

#include "GeneratorBatchTest.h"

class HybridBatchTest : public GeneratorBatchTest
{
public:
    HybridBatchTest(Solver solver);

    void Run(const std::string &filename) override;

private:
    void RunTest(std::ofstream &file, int length, double ratio, double count);
};




#pragma once


#include "GeneratorBatchTest.h"

class QHiddenBatchTest : public GeneratorBatchTest
{
public:
    void Run(const std::string &filename) override;
    QHiddenBatchTest(Solver solver);
private:
    void RunTest(std::ofstream &file, int length, double ratio, double recordCount, int bits);
};




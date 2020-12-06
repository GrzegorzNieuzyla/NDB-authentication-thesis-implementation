#pragma once


#include <vector>
#include <string>
#include "GeneratorBatchTest.h"

class PrefixBatchTest : public GeneratorBatchTest
{
public:
    PrefixBatchTest(Solver solver);
    void Run(const std::string& filename) override;
private:
    void RunTest(std::ofstream& file, int count, int length);
};




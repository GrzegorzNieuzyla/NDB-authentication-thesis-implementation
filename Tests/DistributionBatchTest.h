#pragma once


#include "GeneratorBatchTest.h"

class DistributionBatchTest
{
public:
    static void Run();

private:
    static std::pair<std::unique_ptr<NDB_Generator>, DBRecord> GetGenerator();

};




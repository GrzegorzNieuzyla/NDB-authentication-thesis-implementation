#pragma once


#include "GeneratorBatchTest.h"

class QHiddenBatchTest : public GeneratorBatchTest
{
public:
    void Run(const std::string &filename) override;
    QHiddenBatchTest(Solver solver, std::vector<int> lengths, std::vector<double> probabilityRatios, std::vector<double> recordCountRatios,
                     std::vector<int> definedBits, int repeat);
private:
    std::vector<int> _lengths;
    std::vector<double> _probabilityRatios;
    std::vector<double> _recordCountRatios;
    std::vector<int> _definedBits;
    int _repeat;


    void RunTest(std::ofstream &file, int length, double ratio, double recordCount, int bits);
};




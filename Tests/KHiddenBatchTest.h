#pragma once
#include "GeneratorBatchTest.h"

class KHiddenBatchTest : public GeneratorBatchTest
{
public:
    KHiddenBatchTest(Solver solver, std::vector<int> lengths,
                     std::vector<std::vector<double>> probabilityRatios,
                     std::vector<double> recordCountRatios, std::vector<int> definedBits, int repeat);
    void Run(const std::string &filename) override;

private:
    std::vector<int> _lengths;
    std::vector<std::vector<double>> _probabilityRatios;
    std::vector<double> _recordCountRatios;
    std::vector<int> _definedBits;
    int _repeat;


    void RunTest(std::ofstream &file, int length, const std::vector<double> &prob, double count, int bits);
};




#pragma once


#include <vector>
#include <string>
#include "../Utils/Checksum.h"
#include "../Utils/RandomValuesGenerator.h"

class SuperfluousStringBatchTest
{
public:
    SuperfluousStringBatchTest(std::vector<int> lengths,
                               std::vector<double> recordCountRatios,
                               double probabilityRatio, int specifiedBits,
                               std::vector<Checksum::ChecksumType> checksumTypes, int repeat);

    SuperfluousStringBatchTest(std::vector<int> lengths, std::vector<double> recordCountRatios,
                               std::vector<double> probabilityRatios, int specifiedBits,
                               std::vector<Checksum::ChecksumType> checksumTypes, int repeat);

    void Run(const std::string& filename);
private:
    void RunTestCase(std::ofstream &file, int length, double recordCountRatio, Checksum::ChecksumType checksum);
    enum class Algorithm
    {
        QHidden,
        KHidden
    };
    std::vector<int> _lengths;
    std::vector<double> _recordCountRatios;
    std::vector<double> _probabilityRatios;
    std::vector<Checksum::ChecksumType> _checksumTypes;
    double _probabilityRatio{};
    int _specifiedBits;
    int _repeat;
    Algorithm _algorithm;
};




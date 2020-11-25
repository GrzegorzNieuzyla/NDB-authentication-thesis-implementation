#pragma once
#include "NDB_Generator.h"

class NDB_KHiddenGenerator : public NDB_Generator
{
public:
    NDB_KHiddenGenerator(const DBRecord& record, int length, std::vector<double> probabilityRatios, double recordCountRatio=5.5, int definedPositionCount=3);
    size_t Generate(Stream &output) override;
    void PrintParameters() const override;
    static std::string GetName();
private:
    int GetSpecifiedBitsCount(double rand);
    std::vector<double> _probabilityRatios;
    double _recordCountRatio;
    int _definedPositionCount;
};




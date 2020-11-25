#pragma once
#include "NDB_Generator.h"

class NDB_QHiddenGenerator : public NDB_Generator
{
public:
    NDB_QHiddenGenerator(const DBRecord& record, int length, double probabilityRatio = 0.5, double recordCountRatio = 5.5, int definedPositionCount = 3);
    void PrintParameters() const override;
    size_t Generate(Stream &output) override;
    static std::string GetName();
private:
    double _probabilityRatio;
    double _recordCountRatio;
    int _definedPositionCount;
};




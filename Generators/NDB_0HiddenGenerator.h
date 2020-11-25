#pragma once
#include "NDB_Generator.h"

class NDB_0HiddenGenerator : public NDB_Generator
{
public:
    NDB_0HiddenGenerator(int length, double recordCountRatio, int definedPositionCount);
    void PrintParameters() const override;
    size_t Generate(Stream &output) override;
    static std::string GetName();
private:
    double _recordCountRatio;
    int _definedPositionCount;
};




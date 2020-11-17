#pragma once
#include "NDB_Generator.h"

class NDB_0HiddenGenerator : public NDB_Generator
{
public:
    NDB_0HiddenGenerator(int recordCountRatio, int definedPositionCount);
    size_t Generate(Stream &output) override;
    static std::string GetName();
private:
    int _recordCountRatio;
    int _definedPositionCount;
};




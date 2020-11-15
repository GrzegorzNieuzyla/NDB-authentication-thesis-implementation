#pragma once
#include "NDB_Generator.h"

class NDB_QHiddenGenerator : public NDB_Generator
{
public:
    NDB_QHiddenGenerator(const DBRecord& record, int length, double probabilityRatio = 0.5, double recordCountRatio = 5.5, int definedPositionCount = 3);
    explicit NDB_QHiddenGenerator(int length, double probabilityRatio = 0.5, double recordCountRatio = 5.5, int definedPositionCount = 3);

    size_t Generate(Stream &output) override;
    static std::string GetName();
private:
    size_t Generate0Hidden(Stream& output);
    double _probabilityRatio;
    double _recordCountRatio;
    int _definedPositionCount;
};




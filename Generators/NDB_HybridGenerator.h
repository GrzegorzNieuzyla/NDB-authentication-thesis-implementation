#pragma once
#include "NDB_Generator.h"

class NDB_HybridGenerator : public NDB_Generator
{
public:
    NDB_HybridGenerator(const DBRecord& record, int length, double probabilityRatio = 0.3, double recordCountRatio = 5.5);
    size_t Generate(Stream &output) override;
    static std::string GetName();
private:
    NDB GenComplete(const Permutation& permutation);
    std::size_t MakeHardReverse(const NDB& ndb, Stream& output);
    double _probabilityRatio;
    double _recordCountRatio;
};




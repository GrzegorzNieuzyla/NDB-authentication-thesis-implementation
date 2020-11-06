#pragma once
#include "NDBGenerator.h"
#include "../Utils/RandomValuesGenerator.h"
#include <set>

class NDBRandomizedGenerator : NDBGenerator
{
public:
    NDBRandomizedGenerator(const std::set<DBRecord>& db, int length);
    NDB Generate() override;
private:
    static std::vector<DBRecord> GetAllPatterns(int length);
    [[nodiscard]] bool DoesNDBRecordMatchesAny(std::vector<NDBChar> record) const;
    [[nodiscard]] NDBRecord PatternGenerate(const DBRecord& record) const;
    static bool Matches(VectorView<NDBChar> ndbRecord, VectorView<bool> dbRecord);
    static bool Matches(const std::vector<NDBChar>& ndbRecord, const std::vector<bool>& dbRecord);
    mutable RandomValuesGenerator _random;
};




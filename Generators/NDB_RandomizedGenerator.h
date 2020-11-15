#pragma once
#include "NDB_Generator.h"
#include "../Utils/RandomValuesGenerator.h"
#include <set>

class NDB_RandomizedGenerator : NDB_Generator
{
public:
    NDB_RandomizedGenerator(const std::set<DBRecord>& db, int length);
    std::size_t Generate(Stream& output) override;
    static std::string GetName()
    {
        return "Randomized";
    }

private:
    [[nodiscard]] bool DoesNDBRecordMatchesAny(std::vector<NDBChar> record) const;
    [[nodiscard]] NDBRecord PatternGenerate(const DBRecord& record) const;
    static bool Matches(VectorView<NDBChar> ndbRecord, VectorView<bool> dbRecord);
    static bool Matches(const std::vector<NDBChar>& ndbRecord, const std::vector<bool>& dbRecord, std::vector<int> indices);
    static bool Matches(const std::vector<NDBChar>& ndbRecord, const std::vector<bool>& dbRecord);
};




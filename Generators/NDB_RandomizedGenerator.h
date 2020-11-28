#pragma once
#include "NDB_Generator.h"
#include "../Utils/RandomValuesGenerator.h"
#include <set>
#include "../Streams/Stream.h"

class NDB_RandomizedGenerator : public NDB_Generator
{
public:
    NDB_RandomizedGenerator(const std::set<DBRecord>& db, int length);
    std::size_t Generate(Stream& output) override;
    void PrintParameters() const override;

    static std::string GetName()
    {
        return "Randomized";
    }

    FileUtils::CsvFileData GetCsvData() const override;

private:
    void ApplyPermutationToDb(const Permutation& permutation, bool reverse=false);
    static void GetModifiedRecords(const DBRecord& record, std::vector<int> &bits, int length, std::vector<std::vector<std::pair<int, bool>>>& result);
    [[nodiscard]] bool DoesNDBRecordMatchesAny(std::vector<NDBChar> record) const;
    [[nodiscard]] NDBRecord PatternGenerate(const DBRecord& record, const std::vector<std::pair<int, bool>>& augs) const;
    static bool Matches(VectorView<NDBChar> ndbRecord, VectorView<bool> dbRecord);
    static bool Matches(const std::vector<NDBChar>& ndbRecord, const std::vector<bool>& dbRecord, std::vector<int> indices);
    static bool Matches(const std::vector<NDBChar>& ndbRecord, const std::vector<bool>& dbRecord);
};




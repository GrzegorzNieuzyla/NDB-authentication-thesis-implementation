#include "NDB_RandomizedGenerator.h"
#include "../Utils/NDBUtils.h"
#include <cmath>
#include <iostream>

NDB_RandomizedGenerator::NDB_RandomizedGenerator(const std::set<DBRecord>& db, int length)
 : NDB_Generator(db, length)
{}

std::size_t NDB_RandomizedGenerator::Generate(Stream& output)
{
    std::size_t count = 0;
    int init = std::min(
            RandomValuesGenerator::GetRandomInt(0, ceil(log2(_length))*1.5),
            _length/2);
    auto permutation = RandomValuesGenerator::GenerateRandomPermutation(_length);
    ApplyPermutationToDb(permutation);

    auto W = NDBUtils::GetAllPatterns(init);

    for (int i = init; i < _length && !W.empty(); ++i)
    {
        std::cout << i << std::endl;

        for (const auto& Vp : GetPatternsNotInDBWithPrefixes(W))
        {
            auto rand = RandomValuesGenerator::GetRandomInt(
                    1,
                    std::max(4, static_cast<int>(ceil(log2(_length)) * 2))
                    );
            for (auto j = 1; j <= rand; ++j)
            {
                auto bits = RandomValuesGenerator::GetRandomIndices(_length - Vp.Size(), RandomValuesGenerator::GetRandomInt(0, std::min(
                        static_cast<int>(_length - Vp.Size()),
                        static_cast<int>(ceil(log2(_length))))));
                std::vector<std::vector<std::pair<int, bool>>> bitIndices;
                GetModifiedRecords(Vp, bits, bits.size(), bitIndices);
                for (const auto& augs : bitIndices)
                {
                    output << permutation.Inverse(PatternGenerate(Vp, augs).ToString()) << "\n";
                    count++;
                }
            }
        }
        W = GetPrefixes(i + 1);
    }

    ApplyPermutationToDb(permutation, true);
    return count;
}

NDBRecord NDB_RandomizedGenerator::PatternGenerate(const DBRecord &record, const std::vector<std::pair<int, bool>>& augs) const
{
    auto permutation = RandomValuesGenerator::GenerateRandomPermutation(GetDBRecordSize());
    auto pattern = ToNDBRecord(record, GetDBRecordSize()).Characters();
    for (const auto& aug : augs)
    {
        pattern[aug.first+record.Size()] = aug.second ? NDBChar::Bit1 : NDBChar::Bit0;
    }
    std::vector<std::pair<int, NDBChar>> SIV;
    for (int i = 0; i < pattern.size(); ++i)
    {
        auto permInd = permutation.GetIndex(i);
        auto bit = pattern[permInd];
        if (bit == NDBChar::Wildcard) continue;
        pattern[permInd] = NDBChar::Wildcard;
        if (!DoesNDBRecordMatchesAny(pattern))
        {
            SIV.emplace_back(permInd, bit);
        }
        else
        {
            pattern[permInd] = bit;
        }
    }
    int t = RandomValuesGenerator::GetRandomInt(0, SIV.size());
    auto choices = RandomValuesGenerator::GetRandomChoice(SIV, t);
    for (const auto& pair : choices)
    {
        pattern[pair.first] = pair.second;
    }

    return NDBRecord(pattern);

}

bool NDB_RandomizedGenerator::Matches(const std::vector<NDBChar>& ndbRecord, const std::vector<bool>& dbRecord)
{
    if (ndbRecord.size() != dbRecord.size())
        return false;
    for (int i = 0; i < ndbRecord.size();  ++i)
    {
        auto ndbChar = ndbRecord[i];
        auto dbChar = dbRecord[i];
        if (ndbChar == NDBChar::Wildcard) continue;
        if ((dbChar && ndbChar == NDBChar::Bit0) || (!dbChar && ndbChar == NDBChar::Bit1))
            return false;

    }
    return true;

}

bool NDB_RandomizedGenerator::Matches(VectorView<NDBChar> ndbRecord, VectorView<bool> dbRecord)
{
    if (ndbRecord.Size() != dbRecord.Size())
        return false;
    auto ndbChar = ndbRecord._begin;
    for (const auto& dbChar : dbRecord)
    {
        if ((*ndbChar != NDBChar::Wildcard) && (dbChar && *ndbChar == NDBChar::Bit0) || (!dbChar && *ndbChar == NDBChar::Bit1))
            return false;
        ++ndbChar;
    }
    return true;
}

bool NDB_RandomizedGenerator::Matches(const std::vector<NDBChar>& ndbRecord, const std::vector<bool>& dbRecord, std::vector<int> indices)
{
    if (ndbRecord.size() != dbRecord.size())
        return false;
    for (auto index : indices)
    {
        auto ndbChar = ndbRecord[index];
        auto dbChar = dbRecord[index];
        if (ndbChar == NDBChar::Wildcard) continue;
        if ((dbChar && ndbChar == NDBChar::Bit0) || (!dbChar && ndbChar == NDBChar::Bit1))
            return false;
    }
    return true;
}


bool NDB_RandomizedGenerator::DoesNDBRecordMatchesAny(std::vector<NDBChar> record) const
{
    if (record.empty()) return false;

    std::vector<int> indices;
    indices.reserve(record.size());
    for (int i = 0; i < record.size(); ++i)
    {
        if (record[i] != NDBChar::Wildcard)
        {
            indices.push_back(i);
        }
    }

    return std::any_of(_db.begin(), _db.end(), [&](const auto& dbRecord){
        return Matches(record, dbRecord.Characters(), indices);
    });
}

void NDB_RandomizedGenerator::ApplyPermutationToDb(const Permutation& permutation, bool reverse)
{
    for (auto& record : _db)
    {
        if (reverse)
        {
            record = DBRecord(permutation.Inverse(record.Characters()));
        }
        else
        {
            record = DBRecord(permutation.Apply(record.Characters()));
        }
    }
    std::sort(_db.begin(), _db.end());
}

void NDB_RandomizedGenerator::GetModifiedRecords(const DBRecord &record, std::vector<int> &bits, int length, std::vector<std::vector<std::pair<int, bool>>>& result)
{
    if (bits.empty())
    {
        result.emplace_back();
        result.back().reserve(length);
        return;
    }
    auto index = bits.back();
    bits.pop_back();
    GetModifiedRecords(record, bits, length, result);
    int len = result.size();
    for (int i = 0; i < len; ++i)
    {
        result.push_back(result[i]);
    }
    for (int i = 0; i < result.size(); ++i)
    {
        result[i].push_back({index, i >= result.size()/2});

    }
}

void NDB_RandomizedGenerator::PrintParameters() const
{
    std::cout << "Algorithm: " << GetName() << std::endl;
    std::cout << "c: " << _db.size() << std::endl;
    std::cout << "l: " << _length << std::endl;
}

FileUtils::CsvFileData NDB_RandomizedGenerator::GetCsvData() const
{
    return FileUtils::CsvFileData {.algorithm=GetName(), .length = _length, .count = static_cast<int>(_db.size())};
}




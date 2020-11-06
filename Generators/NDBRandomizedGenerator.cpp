#include "NDBRandomizedGenerator.h"
#include <cmath>
#include <iostream>

NDBRandomizedGenerator::NDBRandomizedGenerator(const std::set<DBRecord>& db, int length)
 : NDBGenerator(db, length)
{}

NDB NDBRandomizedGenerator::Generate()
{
    NDBRecordUnorderedSet ndbRecords;
    int init = ceil(log2(_length));
    auto W = GetAllPatterns(init);

    for (int i = init; i < _length; ++i)
    {
        std::cout << i << std::endl;

        for (const auto& Vp : GetPatternsNotInDBWithPrefixes(W))
        {
            auto rand = _random.GetRandomInt(1, _length);
            for (auto j = 1; j <= rand; ++j)
            {
                ndbRecords.emplace(PatternGenerate(Vp));
            }
        }
        W = GetPrefixes(i + 1);
    }

    NDB ndb;
    ndb.Records() = {ndbRecords.begin(), ndbRecords.end()};
    return ndb;
}

std::vector<DBRecord> NDBRandomizedGenerator::GetAllPatterns(int length)
{
    std::vector<DBRecord> result;
    if (length == 0) return {DBRecord()};

    for (const auto& pattern : GetAllPatterns(length - 1))
    {
        result.push_back(pattern);
        result.back().Characters().push_back(false);
        result.push_back(pattern);
        result.back().Characters().push_back(true);
    }
    return result;
}

NDBRecord NDBRandomizedGenerator::PatternGenerate(const DBRecord &record) const
{
    auto permutation = _random.GenerateRandomPermutation(GetDBRecordSize());
    auto pattern = permutation.Apply<NDBChar>(ToNDBRecord(record, GetDBRecordSize()).Characters());
    std::vector<std::pair<int, NDBChar>> SIV;
    for (int i = 0; i < pattern.size(); ++i)
    {
        auto bit = pattern[i];
        if (bit == NDBChar::Wildcard) continue;
        pattern[i] = NDBChar::Wildcard;
        auto inverse = permutation.Inverse(pattern);
        if (!DoesNDBRecordMatchesAny(inverse))
        {
            SIV.emplace_back(i, bit);
        }
        else
        {
            pattern[i] = bit;
        }
    }
    int t = _random.GetRandomInt(0, 2);
    auto choices = _random.GetRandomChoice(SIV, t);
    for (const auto& pair : choices)
    {
        pattern[pair.first] = pair.second;
    }


    return NDBRecord(permutation.Inverse<NDBChar>(pattern));
}

bool NDBRandomizedGenerator::Matches(VectorView<NDBChar> ndbRecord, VectorView<bool> dbRecord)
{
    if (ndbRecord.Size() != dbRecord.Size())
        return false;
    auto ndbChar = ndbRecord._begin;
    for (const auto& dbChar : dbRecord)
    {
        if (*ndbChar == NDBChar::Wildcard) continue;
        if ((dbChar && *ndbChar == NDBChar::Bit0) || (!dbChar && *ndbChar == NDBChar::Bit1))
            return false;
        ++ndbChar;
    }
    return true;
}

bool NDBRandomizedGenerator::Matches(const std::vector<NDBChar>& ndbRecord, const std::vector<bool>& dbRecord)
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


bool NDBRandomizedGenerator::DoesNDBRecordMatchesAny(std::vector<NDBChar> record) const
{
    return std::any_of(_db.begin(), _db.end(), [&](const auto& dbRecord){
        return Matches(record, dbRecord.Characters());
    });
}


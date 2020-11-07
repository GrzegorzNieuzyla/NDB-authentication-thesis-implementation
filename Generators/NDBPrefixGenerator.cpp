#include <iostream>
#include "NDBPrefixGenerator.h"

NDBPrefixGenerator::NDBPrefixGenerator(const std::set<DBRecord>& db, int length)
        : NDBGenerator(db, length)
{}

NDB NDBPrefixGenerator::Generate()
{
    NDB ndb;
    std::vector<DBRecord> W = {DBRecord()};

    for (int i = 0; i < _length; ++i)
    {
        for (const auto& Vp : GetPatternsNotInDBWithPrefixes(W))
        {
            ndb.Add(ToNDBRecord(Vp, _length));
        }
        W = GetPrefixes(i + 1);
    };
    return ndb;
}

std::size_t NDBPrefixGenerator::GenerateToFile(std::ostream& output)
{
    std::size_t count = 0;
    std::vector<DBRecord> W = {DBRecord()};
    for (int i = 0; i < _length; ++i)
    {
        for (const auto& Vp : GetPatternsNotInDBWithPrefixes(W))
        {
            output << ToNDBRecord(Vp, _length).ToString() << std::endl;
            count++;
        }
        W = GetPrefixes(i + 1);
    };

    return count;
}

#include <iostream>
#include "NDBPrefixGenerator.h"

NDBPrefixGenerator::NDBPrefixGenerator(const std::set<DBRecord>& db, int length)
        : NDBGenerator(db, length)
{}

std::size_t NDBPrefixGenerator::Generate(Stream& output)
{
    std::size_t count = 0;
    std::vector<DBRecord> W = {DBRecord()};
    for (int i = 0; i < _length; ++i)
    {
        for (const auto& Vp : GetPatternsNotInDBWithPrefixes(W))
        {
            output << ToNDBRecord(Vp, _length).ToString() << "\n";
            count++;
        }
        W = GetPrefixes(i + 1);
    };

    return count;
}

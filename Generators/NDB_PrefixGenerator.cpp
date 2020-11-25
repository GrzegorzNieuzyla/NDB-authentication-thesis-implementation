#include <iostream>
#include "NDB_PrefixGenerator.h"

NDB_PrefixGenerator::NDB_PrefixGenerator(const std::set<DBRecord>& db, int length)
        : NDB_Generator(db, length)
{}

std::size_t NDB_PrefixGenerator::Generate(Stream& output)
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

void NDB_PrefixGenerator::PrintParameters() const
{
    std::cout << "Algorithm: " << GetName() << std::endl;
    std::cout << "c: " << _db.size() << std::endl;
    std::cout << "l: " << _length << std::endl;
}


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
            ndb.add(ToNDBRecord(Vp, _length));
        }
        W = GetPrefixes(i + 1);
    };
    return ndb;
}

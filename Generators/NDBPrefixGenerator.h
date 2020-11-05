#include "../NDB.h"
#include "NDBGenerator.h"
#include <set>
#include <unordered_set>

class NDBPrefixGenerator : NDBGenerator
{
public:
    NDBPrefixGenerator(const std::set<DBRecord>& db, int length);

    // Generates NDB from DB
    NDB Generate() override;
};
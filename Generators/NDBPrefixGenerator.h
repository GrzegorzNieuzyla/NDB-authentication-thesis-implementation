#include "../NDB.h"
#include "NDBGenerator.h"
#include <set>
#include <unordered_set>

class NDBPrefixGenerator : NDBGenerator
{
public:
    NDBPrefixGenerator(const std::set<DBRecord>& db, int length);

    [[nodiscard]] static std::string GetName()
    {
        return "Prefix";
    }

    // Generates NDB from DB
    std::size_t GenerateToFile(std::ostream& output) override;

    NDB Generate() override;
};
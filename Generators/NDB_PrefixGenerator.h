#include "../NDB.h"
#include "NDB_Generator.h"
#include <set>
#include <unordered_set>

class NDB_PrefixGenerator : public NDB_Generator
{
public:
    NDB_PrefixGenerator(const std::set<DBRecord>& db, int length);

    void PrintParameters() const override;

    [[nodiscard]] static std::string GetName()
    {
        return "Prefix";
    }

    // Generates NDB from DB
    std::size_t Generate(Stream& output) override;
};
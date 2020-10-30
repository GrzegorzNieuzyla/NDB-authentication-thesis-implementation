#include "../NDB.h"
#include <set>
#include <unordered_set>
#include <vector>
#include "NDBGenerator.h"

class NDBPrefixGenerator : NDBGenerator
{
public:
    NDBPrefixGenerator(std::set<DBRecord> db, int length);

    // Generates NDB from DB
    NDB Generate() override;
private:
    [[nodiscard]] std::vector<DBRecord> GetPrefixesToAdd(DBRecord prefix) const;
    std::unordered_set<DBRecord> GetPatterns(const std::unordered_set<DBRecord>& Wi);

    // Converts DB records prefix to NDB record, pads to 'len' length with '*'
    NDBRecord ToNDBRecord(const DBRecord& record, std::size_t len);

    // Gets all prefixes of length 'len' from DB
    std::unordered_set<DBRecord> GetPrefixes(std::size_t len);

    // Returns iterator bounds of records where 'prefix' is a prefix to
    [[nodiscard]] static std::pair<std::vector<DBRecord>::const_iterator, std::vector<DBRecord>::const_iterator>
        FindPrefixBounds(const std::vector<DBRecord>& db, const DBRecord& prefix);
    std::set<DBRecord> _db;
    std::vector<DBRecord> _dbVector;
    int _length;
};
#include "../NDB.h"
#include <set>
#include <unordered_set>
#include <vector>
#include "NDBGenerator.h"

class NDBPrefixGenerator : NDBGenerator
{
public:
    NDBPrefixGenerator(std::set<DBRecord> db, int length);
    NDB Generate() override;
private:
    [[nodiscard]] std::vector<DBRecord> GetPrefixesToAdd(DBRecord prefix) const;
    std::unordered_set<DBRecord> GetPatterns(const std::unordered_set<DBRecord>& Wi);
    NDBRecord ToNDBRecord(const DBRecord& record, std::size_t len);
    std::unordered_set<DBRecord> GetPrefixes(std::size_t len);
    [[nodiscard]] static std::pair<std::vector<DBRecord>::const_iterator, std::vector<DBRecord>::const_iterator>
        FindPrefixBounds(const std::vector<DBRecord>& db, const DBRecord& prefix);
    std::set<DBRecord> _db;
    int _length;
};
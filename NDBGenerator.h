#include "NDB.h"
#include <set>
#include <unordered_set>
#include <vector>

using DBRecord = std::vector<bool>;

class NDBPrefixGenerator
{
public:
    explicit NDBPrefixGenerator(std::set<DBRecord> db);
    NDB generate(int length);

private:
    [[nodiscard]] std::vector<DBRecord> getPrefixesToAdd(DBRecord prefix) const;
    std::unordered_set<DBRecord> getPatterns(const std::unordered_set<DBRecord>& Wi);
    NDBRecord toNDBRecord(const DBRecord& record, std::size_t len);
    std::unordered_set<DBRecord> getPrefixes(std::size_t len);
    std::set<DBRecord> _db;
    [[nodiscard]] std::pair<std::vector<DBRecord>::const_iterator, std::vector<DBRecord>::const_iterator>
        findPrefixBounds(const std::vector<DBRecord>& db, const DBRecord& prefix) const;

};
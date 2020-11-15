#include "NDB_Generator.h"
#include <cassert>
#include <algorithm>

NDB_Generator::NDB_Generator(const std::set<DBRecord>& db, int length)
    : _length(length)
{
    _db = {db.begin(), db.end()};
    assert(std::is_sorted(_db.begin(), _db.end()));
}


NDBRecord NDB_Generator::ToNDBRecord(const DBRecord& record, std::size_t len)
{
    static NDBChar values[] = {NDBChar::Bit0, NDBChar::Bit1};
    NDBRecord result;
    result.Characters().resize(len);
    int i = 0;
    for (; i < record.Size(); ++i)
    {
        result.Characters()[i] = values[record.Characters()[i]];
    }
    for (; i < len; ++i)
    {
        result.Characters()[i] = NDBChar::Wildcard;
    }
    return result;
}


std::vector<DBRecord> NDB_Generator::GetPrefixes(std::size_t len) const
{
    std::vector<DBRecord> result;
    result.reserve(_db.size());
    for (const auto& record : _db)
    {
        result.emplace_back(record.begin(), std::next(record.begin(), len));
    }
    return result;
}

std::vector<DBRecord> NDB_Generator::GetPatternsNotInDBWithPrefixes(const std::vector<DBRecord>& Wi) const
{
    std::vector<DBRecord> result;
    result.reserve(Wi.size());
    for (const auto& prefix : Wi)
    {
        for(auto& newPrefix : GetPatternsNotInDbWithPrefix(prefix))
        {
            result.emplace_back(std::move(newPrefix));
        }
    }

    return result;
}

std::vector<DBRecord> NDB_Generator::GetPatternsNotInDbWithPrefix(DBRecord prefix) const
{
    auto bounds = FindPrefixBounds(_db, prefix);
    bool _0 = true, _1 = true;
    while(bounds.second != bounds.first)
    {
        if (bounds.first->Characters().size() > prefix.Characters().size())
        {
            if (!bounds.first->Characters().at(prefix.Characters().size()))
                _0 = false;
            else
                _1 = false;
        }
        bounds.first++;
    }

    if (!_0 && !_1)
        return {};
    prefix.Characters().push_back(false);

    std::vector<DBRecord> result;

    if (_0)
    {
        result.push_back(prefix);
    }
    if (_1)
    {
        prefix.Characters().back() = true;
        result.push_back(prefix);
    }
    return result;
}

std::pair<std::vector<DBRecord>::const_iterator, std::vector<DBRecord>::const_iterator>
NDB_Generator::FindPrefixBounds(const std::vector<DBRecord>& db, const DBRecord &prefix)
{
    auto getEqualPrefix = [&](const DBRecord& p){ return DBRecordView(p.begin(), p.begin() + prefix.Size());};
    auto first = 0;
    auto second = db.size() - 1;
    while(second > first)
    {
        auto middle = first + (second - first + 1) / 2;
        auto middlePrefix = getEqualPrefix(db[middle]);
        if (second - first == 1)
        {
            if (getEqualPrefix(db[first]) == prefix.Characters())
            {
                second = first;
                break;
            }
            else
            {
                first = second;
                break;
            }
        }
        if (middlePrefix == prefix.Characters())
        {
            second = middle;
            first = middle;
        }
        else if (middlePrefix > prefix.Characters())
        {
            second = middle;
        }
        else
        {
            first = middle;
        }
    }
    assert(second == first);

    if (getEqualPrefix(db[second]) != prefix.Characters())
        return {db.end(), db.end()};

    while(first > 0 && getEqualPrefix(db[first-1]) == prefix.Characters())
        first--;
    while(second < (db.size() - 1) && getEqualPrefix(db[second+1]) == prefix.Characters())
        second++;


    return {db.begin() + first, db.begin() + second + 1};
}

std::size_t NDB_Generator::GetDBRecordSize() const
{
    return _db.empty() ? 0 : _db[0].Size();
}

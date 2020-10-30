#include "NDBPrefixGenerator.h"
#include <algorithm>
#include <iostream>
#include <cassert>

NDBPrefixGenerator::NDBPrefixGenerator(std::set<DBRecord> db, int length)
: _db(std::move(db)), _length(length)
{
    assert(std::is_sorted(_db.begin(), _db.end()));
    _dbVector = {_db.begin(), _db.end()};
}


NDB NDBPrefixGenerator::Generate()
{
    NDB ndb;
    std::unordered_set<DBRecord> W = {DBRecord()};
    
    for (int i = 0; i < _length; ++i)
    {
        for (const auto& Vp : GetPatterns(W))
        {
            ndb.add(ToNDBRecord(Vp, _length));
        }
        W = GetPrefixes(i + 1);
    };

    return ndb;
}

NDBRecord NDBPrefixGenerator::ToNDBRecord(const DBRecord& record, std::size_t len)
{
    static NDBChar values[] = {NDBChar::Bit0, NDBChar::Bit1};
    NDBRecord result;
    result.Characters().resize(len);
    int i = 0;
    for (; i < record.size(); ++i)
    {
        result.Characters()[i] = values[record[i]];
    }
    for (; i < len; ++i)
    {
        result.Characters()[i] = NDBChar::Wildcard;
    }
    return result;
}


std::unordered_set<DBRecord> NDBPrefixGenerator::GetPrefixes(std::size_t len)
{
    std::unordered_set<DBRecord> result;
    result.reserve(_db.size());
    for (const auto& record : _db)
    {
        result.emplace(record.begin(), std::next(record.begin(), len));
    }
    return result;
}

std::unordered_set<DBRecord> NDBPrefixGenerator::GetPatterns(const std::unordered_set<DBRecord>& Wi)
{
    std::unordered_set<DBRecord> result;
    result.reserve(Wi.size());
    for (const auto& prefix : Wi)
    {
        for(auto& newPrefix : GetPrefixesToAdd(prefix))
        {
            result.emplace(std::move(newPrefix));
        }
    }

    return result;
}

std::vector<DBRecord> NDBPrefixGenerator::GetPrefixesToAdd(DBRecord prefix) const
{
    auto bounds = FindPrefixBounds(_dbVector, prefix);
    bool _0 = true, _1 = true;
    while(bounds.second != bounds.first)
    {
        if (bounds.first->size() > prefix.size())
        {
            if (!bounds.first->at(prefix.size()))
                _0 = false;
            else
                _1 = false;
        }
        bounds.first++;
    }

    if (!_0 && !_1)
        return {};
    prefix.push_back(false);

    std::vector<DBRecord> result;

    if (_0)
    {
        result.push_back(prefix);
    }
    if (_1)
    {
        prefix.back() = true;
        result.push_back(prefix);
    }
    return result;
}

std::pair<std::vector<DBRecord>::const_iterator, std::vector<DBRecord>::const_iterator>
NDBPrefixGenerator::FindPrefixBounds(const std::vector<DBRecord>& db, const DBRecord &prefix)
{
    auto getEqualPrefix = [&](const DBRecord& p){ return DBRecordView(p.begin(), p.begin() + prefix.size());};
    auto first = 0;
    auto second = db.size() - 1;
    while(second > first)
    {
        auto middle = first + (second - first + 1) / 2;
        auto middlePrefix = getEqualPrefix(db[middle]);
        if (second - first == 1)
        {
            if (getEqualPrefix(db[first]) == prefix)
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
        if (middlePrefix == prefix)
        {
            second = middle;
            first = middle;
        }
        else if (middlePrefix > prefix)
        {
            second = middle;
        }
        else
        {
            first = middle;
        }
    }
    assert(second == first);

    if (getEqualPrefix(db[second]) != prefix)
        return {db.end(), db.end()};

    while(first > 0 && getEqualPrefix(db[first-1]) == prefix)
        first--;
    while(second < (db.size() - 1) && getEqualPrefix(db[second+1]) == prefix)
        second++;


    return {db.begin() + first, db.begin() + second + 1};
}

#include "NDBGenerator.h"
#include <algorithm>
#include <iostream>
#include <cassert>

NDBPrefixGenerator::NDBPrefixGenerator(std::set<DBRecord> db)
: _db(std::move(db))
{
    assert(std::is_sorted(db.begin(), db.end()));
}


NDB NDBPrefixGenerator::generate(int length)
{
    NDB ndb;
    std::unordered_set<DBRecord> W = {DBRecord()};
    
    for (int i = 0; i < length; ++i)
    {
        std::cout << i << std::endl;
        for (const auto& Vp : getPatterns(W))
        {
            ndb.add(toNDBRecord(Vp, length));
        }
        W = getPrefixes(i + 1);
    };

    return ndb;
}

NDBRecord NDBPrefixGenerator::toNDBRecord(const DBRecord& record, std::size_t len)
{
    NDBRecord result;
    result.characters().reserve(len);
    for (int i = 0; i < len; ++i)
    {
        if (i < record.size())
            result.characters().push_back(record[i] ? NDBChar::Bit1 : NDBChar::Bit0);
        else
            result.characters().push_back(NDBChar::Wildcard);
    }
    return result;
}


std::unordered_set<DBRecord> NDBPrefixGenerator::getPrefixes(std::size_t len)
{
    std::unordered_set<DBRecord> result;
    result.reserve(_db.size());
    for (const auto& record : _db)
    {
        result.emplace(record.begin(), std::next(record.begin(), len));
    }
    return result;
}

std::unordered_set<DBRecord> NDBPrefixGenerator::getPatterns(const std::unordered_set<DBRecord>& Wi)
{
    std::unordered_set<DBRecord> result;
    result.reserve(Wi.size());
    for (const auto& prefix : Wi)
    {
        for(auto& newPrefix : getPrefixesToAdd(prefix))
        {
            result.emplace(std::move(newPrefix));
        }
    }

    return result;
}

std::vector<DBRecord> NDBPrefixGenerator::getPrefixesToAdd(DBRecord prefix) const
{

    const std::vector<DBRecord> db(_db.begin(), _db.end());
    auto bounds = findPrefixBounds(db, prefix);
    bool _0 = true, _1 = true;
    while(bounds.first != db.end() && bounds.second >= bounds.first)
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
NDBPrefixGenerator::findPrefixBounds(const std::vector<DBRecord>& db, const DBRecord &prefix) const
{
    auto getEqualPrefix = [&](const DBRecord& p){ return DBRecord(p.begin(), p.begin() + prefix.size());};
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


    return {db.begin() + first, db.begin() + second};
}

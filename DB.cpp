#include "DB.h"
#include <utility>
#include <sstream>
#include <cassert>

DBRecord::DBRecord(std::vector<bool> record) : _record(std::move(record))
{}

const std::vector<bool> &DBRecord::Characters() const
{
    return _record;
}


std::vector<bool> &DBRecord::Characters()
{
    return _record;
}

std::vector<bool>::iterator DBRecord::begin() noexcept
{
    return _record.begin();
}

std::vector<bool>::const_iterator DBRecord::begin() const noexcept
{
    return _record.cbegin();
}

std::vector<bool>::const_iterator DBRecord::end() const noexcept
{
    return _record.cend();
}

std::vector<bool>::iterator DBRecord::end() noexcept
{
    return _record.end();
}

DBRecord::DBRecord(std::initializer_list<bool> list) :
_record(list)
{}

bool DBRecord::operator<(const DBRecord& other) const
{
    return _record < other._record;
}

DBRecord::DBRecord(std::vector<bool>::iterator begin, std::vector<bool>::iterator end)
: _record(begin, end)
{

}

size_t DBRecord::Size() const
{
    return _record.size();
}

std::string DBRecord::ToString() const
{
    std::stringstream ss;
    for (const auto ch : *this)
    {
        ss << (ch ? "1" : "0");
    }
    return ss.str();
}

DBRecord::DBRecord(std::vector<bool>::const_iterator begin, std::vector<bool>::const_iterator end)
: _record(begin, end)
{

}

std::vector<unsigned char> DBRecord::ToBytes() const
{
    assert(Size() % 8 == 0);
    std::vector<unsigned char> result;
    result.resize(Size() / 8);
    for (int i = 0; i < Size() / 8; ++i)
    {
        result[i] = 0;
        for (unsigned char j = 0; j < 8; ++j)
        {
            if (Characters()[8*i + j])
                result[i] |= static_cast<unsigned char>(1u << (7 - j));
        }
    }

    return result;
}

void DBRecord::AppendBytes(const std::vector<unsigned char>& bytes)
{
    for (auto byte : bytes)
    {
        for (int i = 7; i >= 0; --i)
        {
            Characters().push_back((byte & (1u << static_cast<unsigned>(i))) > 0);
        }
    }
}

DBRecord DBRecord::FromString(const std::string &string)
{
    DBRecord record;
    for (auto ch : string)
    {
        if (ch != '0' && ch != '1') throw std::invalid_argument("Invalid string");
        record.Characters().push_back(ch == '1');
    }

    return record;
}

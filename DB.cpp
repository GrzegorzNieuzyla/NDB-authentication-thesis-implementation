#include "DB.h"
#include <utility>
#include <sstream>

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

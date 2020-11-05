#include "NDB.h"
#include <sstream>
#include <utility>



NDBChar& NDBRecord::operator[](int index)
{
    return _characters[index];
}

std::size_t NDBRecord::Size() const
{
    return _characters.size();
}

NDBRecord::NDBRecord(std::vector<NDBChar> characters) 
    : _characters(std::move(characters))
    {}

std::vector<NDBChar>& NDBRecord::Characters()
{
    return _characters;
}

const std::vector<NDBChar>& NDBRecord::Characters() const
{
    return _characters;
}

std::string NDBRecord::ToString() const
{
    static char chars[] = {'0', '1', '*'};
    std::stringstream ss;
    for (const auto& ch : Characters())
    {
        ss << chars[static_cast<int>(ch) - 1];
    }
    return ss.str();
}


void NDB::add(NDBRecord record)
{
    _records.emplace_back(std::move(record));
}


std::vector<NDBRecord>& NDB::Records()
{
    return _records;
}

const std::vector<NDBRecord> &NDB::Records() const
{
    return _records;
}

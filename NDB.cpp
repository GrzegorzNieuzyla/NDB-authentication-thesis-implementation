#include "NDB.h"



NDBChar& NDBRecord::operator[](int index)
{
    return _characters[index];
}

std::size_t NDBRecord::Size() const
{
    return _characters.size();
}

NDBRecord::NDBRecord(std::vector<NDBChar> characters) 
    : _characters(characters)
    {}

std::vector<NDBChar>& NDBRecord::Characters()
{
    return _characters;
}


void NDB::add(NDBRecord record)
{
    _records.emplace_back(std::move(record));
}


std::vector<NDBRecord>& NDB::Records()
{
    return _records;
}

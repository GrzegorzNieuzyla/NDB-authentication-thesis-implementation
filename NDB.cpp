#include "NDB.h"
#include <sstream>
#include <utility>
#include <fstream>


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

bool NDBRecord::operator==(const NDBRecord& other) const
{
    return Characters() == other.Characters();
}


void NDB::Add(NDBRecord record)
{
    _records.emplace(std::move(record));
}


NDBRecordUnorderedSet& NDB::Records()
{
    return _records;
}

const NDBRecordUnorderedSet &NDB::Records() const
{
    return _records;
}

std::size_t NDB::Size() const
{
    return _records.size();
}

void NDB::DumpToFile(const std::string& filename) const
{
    std::ofstream file(filename, std::ios::out);
    for (const auto& record : Records())
    {
        file << record.ToString() << std::endl;
    }
    file.close();
}

#include "NDBStream.h"


const NDB &NDBStream::Ndb() const
{
    return _ndb;
}

Stream &NDBStream::operator<<(const std::string &string)
{
    NDBRecord record;
    record.Characters().reserve(string.length());
    for (const auto& ch : string)
    {
        if (ch == '0')
            record.Characters().emplace_back(NDBChar::Bit0);
        else if (ch == '1')
            record.Characters().emplace_back(NDBChar::Bit1);
        else if (ch == '*')
            record.Characters().emplace_back(NDBChar::Wildcard);
    }
    if (!record.Characters().empty())
    {
        _ndb.Add(std::move(record));
    }
    return *this;
}

#include "NDBValidator.h"
#include <utility>
#include <iostream>

NDBValidator::NDBValidator(NDB ndb, DB db) : _ndb(std::move(ndb)), _db(std::move(db))
{}

bool NDBValidator::ValidateAllDBRecords(bool printErrors) const
{
    bool result = true;

    for (const auto& record : _db)
    {
        auto errors = ValidateRecord(record);
        if (!errors.empty())
        {
            result = false;
            if (printErrors)
            {
                for (const auto& error : errors)
                {
                    std::cout << "DB:" << error.first.ToString() << " matched by NDB:" << error.second.ToString() << std::endl;
                }
            }
        }
    }

    return result;
}

bool NDBValidator::ValidateRecordSingle(const DBRecord &dbRecord, const NDBRecord& ndbRecord)
{
    if (dbRecord.Characters().size() != ndbRecord.Size())
        return false;
    for (int i = 0; i < dbRecord.Characters().size(); ++i)
    {
        auto dbChar = dbRecord.Characters()[i];
        auto ndbChar = ndbRecord.Characters()[i];
        if (ndbChar != NDBChar::Wildcard && ((ndbChar == NDBChar::Bit0 && dbChar) || (ndbChar == NDBChar::Bit1 && !dbChar)))
            return true;
    }

    return false;
}

std::vector<std::pair<DBRecord, NDBRecord>> NDBValidator::ValidateRecord(const DBRecord &record) const
{
    std::vector<std::pair<DBRecord, NDBRecord>> errors;
    for (const auto& ndbRecord : _ndb.Records())
    {
        if (!ValidateRecordSingle(record, ndbRecord))
        {
            errors.emplace_back(record, ndbRecord);
        }
    }
    return errors;
}

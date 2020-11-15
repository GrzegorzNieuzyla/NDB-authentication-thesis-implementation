#include "NDBValidator.h"
#include "../Generators/NDB_RandomizedGenerator.h"
#include "../Utils/NDBUtils.h"
#include <utility>
#include <iostream>
#include <fstream>
#include <cassert>

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

bool NDBValidator::ValidateFromFile(const DB& db, const std::string& filename, bool printErrors)
{
    if (db.empty()) return true;
    bool result = true;
    auto recordSize = db.begin()->Size();
    std::ifstream file(filename);
    std::string line;

    while(!file.eof())
    {
        std::getline(file, line);
        if (line.empty()) break;
        assert(line.size() == recordSize);
        auto ndbRecord = StringToNDBRecord(line);
        for (const auto& dbRecord : db)
        {
            if (!ValidateRecordSingle(dbRecord, ndbRecord))
            {
                result = false;

                if (printErrors)
                {
                    std::cout << "DB:" << dbRecord.ToString() << " matched by NDB:" << ndbRecord.ToString() << std::endl;
                }
            }
        }
    }



    return result;
}

NDBRecord NDBValidator::StringToNDBRecord(const std::string &str)
{
    NDBRecord record;
    for (const auto& ch : str)
    {
        if (ch == '0')
            record.Characters().push_back(NDBChar::Bit0);
        else if (ch == '1')
            record.Characters().push_back(NDBChar::Bit1);
        else if (ch == '*')
            record.Characters().push_back(NDBChar::Wildcard);
        else
            assert(false);
    }
    return record;
}

bool NDBValidator::ValidateFalsePositives(bool printSummary, bool printErrors) const
{
    bool result = true;
    int count = 0;
    auto patterns = NDBUtils::GetAllPatterns(_ndb.Size() == 0 ? 0 : static_cast<int>(_ndb.Records().begin()->Size()));
    for (const auto& pattern : patterns)
    {
        if (_db.find(pattern) == _db.end())
        {
            auto errors =  ValidateRecord(pattern);
            if (errors.empty())
            {
                result = false;
                if (printErrors)
                {
                    std::cout << "Non-DB Record: " + pattern.ToString() << " matches" << std::endl;
                }
                count++;
            }
        }
    }
    if (printSummary)
    {
        std::cout << count << " false positives\n";
    }
    return result;
}

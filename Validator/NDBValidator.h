#pragma once
#include "../NDB.h"
#include "../DB.h"

class NDBValidator
{
public:
    NDBValidator(NDB ndb, DB db);
    bool ValidateAllDBRecords(bool printErrors = true) const;
    static bool ValidateFromFile(const DB& db, const std::string& filename, bool printErrors = true);
    bool ValidateFalsePositives(bool printSummary = true, bool printErrors = true) const;
private:
    static NDBRecord StringToNDBRecord(const std::string& str);
    [[nodiscard]] std::vector<std::pair<DBRecord, NDBRecord>> ValidateRecord(const DBRecord& record) const;
    static bool ValidateRecordSingle(const DBRecord& dbRecord, const NDBRecord& ndbRecord) ;
    NDB _ndb;
    DB _db;
};





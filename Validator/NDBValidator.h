#pragma once
#include "../NDB.h"
#include "../DB.h"

class NDBValidator
{
public:
    NDBValidator(NDB ndb, DB db);
    bool ValidateAllDBRecords(bool printErrors = true) const;

private:
    [[nodiscard]] std::vector<std::pair<DBRecord, NDBRecord>> ValidateRecord(const DBRecord& record) const;
    static bool ValidateRecordSingle(const DBRecord& dbRecord, const NDBRecord& ndbRecord) ;
    NDB _ndb;
    DB _db;
};





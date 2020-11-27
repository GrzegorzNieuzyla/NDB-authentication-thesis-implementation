#pragma once
#include "SAT.h"
#include "../DB.h"
#include "../NDB.h"

class ZChaffInterface
{
public:
    ZChaffInterface();
    std::optional<DBRecord> Solve(const NDB& ndb);
    virtual ~ZChaffInterface();

private:
    SAT_Manager _manager;
};




#pragma once
#include "SAT.h"
#include "../DB.h"
#include "../NDB.h"

class ZChaffInterface
{
public:
    struct Result
    {
        std::optional<DBRecord> record;
        std::size_t decisions;
        std::size_t seconds;
    };
    ZChaffInterface();
    Result Solve(const NDB& ndb);
    virtual ~ZChaffInterface();

private:
    SAT_Manager _manager;
};




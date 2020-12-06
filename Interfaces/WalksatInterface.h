#pragma once
#include "../NDB.h"
extern "C"
{
    #include "Walksat.h"
}

class WalksatInterface
{
public:
    WalksatResult Solve(const NDB& ndb, int noise=50);

private:
    std::string TMP_NDB_FILE = ".walksat_tmp_ndb";
};




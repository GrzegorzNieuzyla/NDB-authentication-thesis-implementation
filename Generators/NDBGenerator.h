#pragma once
#include <vector>
#include "../NDB.h"

using DBRecord = std::vector<bool>;

class NDBGenerator
{
public:
    virtual NDB Generate() = 0;
};


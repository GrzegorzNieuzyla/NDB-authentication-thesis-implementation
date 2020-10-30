#pragma once
#include <vector>
#include "../NDB.h"
#include "../VectorView.h"

using DBRecord = std::vector<bool>;
using DBRecordView = VectorView<bool>;
class NDBGenerator
{
public:
    virtual NDB Generate() = 0;
};


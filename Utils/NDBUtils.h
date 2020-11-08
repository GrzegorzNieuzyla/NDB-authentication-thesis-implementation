#pragma once
#include <vector>
#include "../DB.h"

class NDBUtils
{
public:
    static std::vector<DBRecord> GetAllPatterns(int length)
    {
        std::vector<DBRecord> result;
        if (length == 0) return {DBRecord()};

        for (const auto& pattern : GetAllPatterns(length - 1))
        {
            result.push_back(pattern);
            result.back().Characters().push_back(false);
            result.push_back(pattern);
            result.back().Characters().push_back(true);
        }
        return result;
    }
};




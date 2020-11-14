#pragma once
#include <vector>
#include "../DB.h"

class NDBUtils
{
public:
    static std::vector<DBRecord> GetAllPatterns(int length)
    {
        if (length == 0) return {DBRecord()};
        std::vector<DBRecord> result;

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




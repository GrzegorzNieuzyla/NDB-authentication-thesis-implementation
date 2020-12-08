#pragma once

#include <unordered_map>
#include "../NDB.h"
#include "../DB.h"
class DistributionTest
{
public:
    enum class Case
    {
        Equal,
        NotEqual,
        Wildcard
    };
    static std::unordered_map<NDBChar, std::vector<double>> CalculateProbability(const NDB& ndb);
    static std::unordered_map<NDBChar, std::vector<double>> CalculateProbabilityForDefined(const NDB& ndb);
    static std::vector<double> CalculateEntropy(const NDB& ndb);
    static std::vector<double> CalculateEntropyForDefined(const NDB& ndb);
    static std::vector<double> CalculateGini(const NDB &ndb);
    static std::vector<double> CalculateGiniForDefined(const NDB &ndb);
    static std::unordered_map<Case, double> CalculateTotalProbability(const NDB& ndb, const DBRecord& record);
};




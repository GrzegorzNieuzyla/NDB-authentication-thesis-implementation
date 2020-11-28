#pragma once

#include <unordered_map>
#include "../NDB.h"

class DistributionTest
{
public:
    static std::unordered_map<NDBChar, std::vector<double>> CalculateProbability(const NDB& ndb);
    static std::unordered_map<NDBChar, std::vector<double>> CalculateProbabilityForDefined(const NDB& ndb);
    static std::vector<double> CalculateEntropy(const NDB& ndb);
    static std::vector<double> CalculateEntropyForDefined(const NDB& ndb);
    static std::vector<double> CalculateGini(const NDB &ndb);
    static std::vector<double> CalculateGiniForDefined(const NDB &ndb);
};




#include <cassert>
#include "DistributionTest.h"
#include <cmath>
#include <numeric>

std::unordered_map<NDBChar, std::vector<double>> DistributionTest::CalculateProbability(const NDB &ndb)
{
    assert(ndb.Size() != 0 && !ndb.Records().empty());
    auto length = ndb.Records().begin()->Size();
    std::unordered_map<NDBChar, std::vector<double>> result;
    result[NDBChar::Bit0] = std::vector<double>(length, 0);
    result[NDBChar::Bit1] = std::vector<double>(length, 0);
    result[NDBChar::Wildcard] = std::vector<double>(length, 0);

    for (const auto& record : ndb.Records())
    {
        for (int i = 0; i < record.Size(); ++i)
            ++result[record.Characters()[i]][i];
    }
    for (int i = 0; i < length; ++i)
    {
        result[NDBChar::Bit0][i] /= ndb.Size();
        result[NDBChar::Bit1][i] /= ndb.Size();
        result[NDBChar::Wildcard][i] /= ndb.Size();
    }
    return result;
}

std::vector<double> DistributionTest::CalculateEntropy(const NDB &ndb)
{
    auto probability = CalculateProbability(ndb);
    std::vector<double> result(ndb.Records().begin()->Size(), 0.0);
    for(int i = 0; i < ndb.Records().begin()->Size(); ++i)
    {
        double p1 = probability[NDBChar::Bit0][i], p2 = probability[NDBChar::Bit1][i], p3 = probability[NDBChar::Wildcard][i];
        if (p1 == 0 && p2 == 0 && p3 == 0)
            result[i] = 1;
        else if (p1 == 0 || p2 == 0 || p3 == 0)
            result[i] = 0;
        else
            result[i] = -(p1 * log2(p1) + p2 * log2(p2) + p3 * log2(p3)) / log2(3.0);
    }

    return result;
}

std::vector<double> DistributionTest::CalculateGini(const NDB &ndb)
{
    auto probability = CalculateProbability(ndb);
    std::vector<double> result(ndb.Records().begin()->Size(), 0.0);
    for(int i = 0; i < ndb.Records().begin()->Size(); ++i)
    {
        double p1 = probability[NDBChar::Bit0][i], p2 = probability[NDBChar::Bit1][i], p3 = probability[NDBChar::Wildcard][i];
        if (p1 == 0 && p2 == 0 && p3 == 0)
            result[i] = 0.5;
        else if (p1 == 0 || p2 == 0 || p3 == 0)
            result[i] = 0;
        else
            result[i] = 1 - (p1 * p1 + p2 * p2 + p3 * p3);
    }

    return result;
}

std::vector<double> DistributionTest::CalculateEntropyForDefined(const NDB &ndb)
{
    auto probability = CalculateProbabilityForDefined(ndb);
    std::vector<double> result(ndb.Records().begin()->Size(), 0.0);
    for(int i = 0; i < ndb.Records().begin()->Size(); ++i)
    {
        double p1 = probability[NDBChar::Bit0][i], p2 = probability[NDBChar::Bit1][i];
        if (p1 == 0 && p2 == 0)
            result[i] = 1;
        else if (p1 == 0 || p2 == 0)
            result[i] = 0;
        else
            result[i] = -(p1 * log2(p1) + p2 * log2(p2));
    }

    return result;
}

std::vector<double> DistributionTest::CalculateGiniForDefined(const NDB &ndb)
{
    auto probability = CalculateProbabilityForDefined(ndb);
    std::vector<double> result(ndb.Records().begin()->Size(), 0.0);
    for(int i = 0; i < ndb.Records().begin()->Size(); ++i)
    {
        double p1 = probability[NDBChar::Bit0][i], p2 = probability[NDBChar::Bit1][i];
        if (p1 == 0 && p2 == 0)
            result[i] = 0.5;
        else if (p1 == 0 || p2 == 0)
            result[i] = 0;
        else
            result[i] = 1 - (p1 * p1 + p2 * p2);
    }

    return result;
}

std::unordered_map<NDBChar, std::vector<double>> DistributionTest::CalculateProbabilityForDefined(const NDB &ndb)
{
    assert(ndb.Size() != 0 && !ndb.Records().empty());
    auto length = ndb.Records().begin()->Size();
    std::unordered_map<NDBChar, std::vector<double>> result;
    result[NDBChar::Bit0] = std::vector<double>(length, 0);
    result[NDBChar::Bit1] = std::vector<double>(length, 0);

    for (const auto& record : ndb.Records())
    {
        for (int i = 0; i < record.Size(); ++i)
            if (record.Characters()[i] != NDBChar::Wildcard)
            ++result[record.Characters()[i]][i];
    }
    for (int i = 0; i < length; ++i)
    {
        int size = 0;
        for (const auto& record : ndb.Records())
            if (record.Characters()[i] != NDBChar::Wildcard)
                size++;

        result[NDBChar::Bit0][i] /= size;
        result[NDBChar::Bit1][i] /= size;
    }
    return result;
}

std::unordered_map<DistributionTest::Case, double> DistributionTest::CalculateTotalProbability(const NDB &ndb, const DBRecord &record)
{
    std::unordered_map<Case, double> result = {
            {Case::Wildcard, 0},
            {Case::Equal, 0},
            {Case::NotEqual, 0}
    };
    auto total = record.Size() * ndb.Size();
    assert(ndb.Records().begin()->Size() == record.Size());
    for (const auto& ndbRecord : ndb.Records())
    {
        for (int i = 0; i < record.Size(); ++i)
        {
            if (ndbRecord.Characters()[i] == NDBChar::Wildcard)
                result[Case::Wildcard]++;
            else if ((ndbRecord.Characters()[i] == NDBChar::Bit1 && record.Characters()[i]) || (ndbRecord.Characters()[i] == NDBChar::Bit0 && !record.Characters()[i]))
                result[Case::Equal]++;
            else
                result[Case::NotEqual]++;
        }
    }
    result[Case::Wildcard] /= total;
    result[Case::NotEqual] /= total;
    result[Case::Equal] /= total;
    return result;
}


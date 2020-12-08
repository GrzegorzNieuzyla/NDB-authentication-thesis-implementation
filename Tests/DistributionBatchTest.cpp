#include <boost/algorithm/string.hpp>
#include "DistributionBatchTest.h"
#include "../CLIInterface.h"
#include "../Generators/NDB_RandomizedOldGenerator.h"
#include "../Generators/NDB_RandomizedGenerator.h"
#include "../Generators/NDB_0HiddenGenerator.h"
#include "../Generators/NDB_QHiddenGenerator.h"
#include "../Generators/NDB_KHiddenGenerator.h"
#include "../Generators/NDB_HybridGenerator.h"
#include "../Generators/NDB_PrefixGenerator.h"
#include "../Streams/NDBStream.h"
#include "DistributionTest.h"
namespace alg = boost::algorithm;


void DistributionBatchTest::Run()
{
    auto repeat = CLIInterface::GetSettings().repeat;
    std::vector<std::unordered_map<DistributionTest::Case, double>> dist;
    for (int i = 0; i < repeat; ++i)
    {
        auto generator = GetGenerator();
        NDBStream stream;
        generator.first->Generate(stream);
        dist.emplace_back(DistributionTest::CalculateTotalProbability(stream.Ndb(), generator.second));
    }
    double distWildcard = 0;
    double distEqual = 0;
    double distNotEqual = 0;

    for (auto testCase : dist)
    {
        distWildcard += testCase[DistributionTest::Case::Wildcard];
        distEqual += testCase[DistributionTest::Case::Equal];
        distNotEqual += testCase[DistributionTest::Case::NotEqual];
    }
    distWildcard /= dist.size();
    distEqual /= dist.size();
    distNotEqual /= dist.size();
    std::cout << "Equal: " << distEqual << std::endl;
    std::cout << "Not equal: " << distNotEqual << std::endl;
    std::cout << "Wildcard: " << distWildcard << std::endl;
}

std::pair<std::unique_ptr<NDB_Generator>, DBRecord> DistributionBatchTest::GetGenerator()
{
    const auto& settings = CLIInterface::GetSettings();
    auto algorithm = settings.algorithm;
    auto db = RandomValuesGenerator::GenerateRandomDB(1, settings.recordLength[0]);
    if (algorithm == alg::to_lower_copy(NDB_PrefixGenerator::GetName()))
    {
        return {std::unique_ptr<NDB_Generator>(new NDB_PrefixGenerator(db, settings.recordLength[0])), *db.begin()};
    }
    else if (algorithm == alg::to_lower_copy(NDB_RandomizedOldGenerator::GetName()))
    {
        return  {std::unique_ptr<NDB_Generator>(new NDB_RandomizedOldGenerator(db, settings.recordLength[0])), *db.begin()};
    }
    else if (algorithm == alg::to_lower_copy(NDB_RandomizedGenerator::GetName()))
    {
        return  {std::unique_ptr<NDB_Generator>(new NDB_RandomizedGenerator(db, settings.recordLength[0])), *db.begin()};
    }
    else if (algorithm == alg::to_lower_copy(NDB_0HiddenGenerator::GetName()))
    {
        return  {std::unique_ptr<NDB_Generator>(new NDB_0HiddenGenerator(settings.recordLength[0], settings.recordCountRatio[0], settings.specifiedBits[0])), *db.begin()};
    }
    else if (algorithm == alg::to_lower_copy(NDB_QHiddenGenerator::GetName()))
    {
        return  {std::unique_ptr<NDB_Generator>(new NDB_QHiddenGenerator(*db.begin(), settings.recordLength[0], settings.probabilityRatio[0], settings.recordCountRatio[0], settings.specifiedBits[0])), *db.begin()};
    }
    else if (algorithm == alg::to_lower_copy(NDB_KHiddenGenerator::GetName()))
    {
        return {std::unique_ptr<NDB_Generator>(new NDB_KHiddenGenerator(*db.begin(), settings.recordLength[0], settings.probabilityRatios, settings.recordCountRatio[0], settings.specifiedBits[0])), *db.begin()};
    }
    else if (algorithm == alg::to_lower_copy(NDB_HybridGenerator::GetName()))
    {
        return {std::unique_ptr<NDB_Generator>(new NDB_HybridGenerator(*db.begin(), settings.recordLength[0], settings.probabilityRatio[0], settings.recordCountRatio[0])), *db.begin()};
    }
    else
    {
        std::cerr << "No such algorithm: " << algorithm << std::endl;
        exit(1);
    }
}


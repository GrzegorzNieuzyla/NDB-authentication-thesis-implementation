#include "GenerationTimeTest.h"
#include "../CLIInterface.h"
#include "../Generators/NDB_PrefixGenerator.h"
#include "../Generators/NDB_QHiddenGenerator.h"
#include "../Generators/NDB_KHiddenGenerator.h"
#include "../Generators/NDB_HybridGenerator.h"
#include "../Streams/DummyStream.h"
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <cassert>
namespace alg = boost::algorithm;

void GenerationTimeTest::RunTests(const std::string& filename)
{
    std::ofstream file(filename);
    assert(file.is_open());

    auto settings = CLIInterface::GetSettings();
    auto& algorithm = settings.algorithm;
    if (algorithm == alg::to_lower_copy(NDB_QHiddenGenerator::GetName()))
    {
        TestQHidden(file);
    }
    else if (algorithm == alg::to_lower_copy(NDB_KHiddenGenerator::GetName()))
    {
        TestKHidden(file);
    }
    else if (algorithm == alg::to_lower_copy(NDB_HybridGenerator::GetName()))
    {
        TestHybrid(file);
    }
    else
    {
        std::cerr << "Invalid algorithm: " << algorithm << std::endl;
        exit(1);
    }
}

void GenerationTimeTest::TestQHidden(std::ofstream& file)
{
    auto settings = CLIInterface::GetSettings();
    file << FileUtils::CsvFileData::GetCsvDataHeader() << ";gen_time" << std::endl;
    for (auto length : settings.recordLength)
    {
        for (auto definedBits : settings.specifiedBits)
        {
            for (auto recordCount : settings.recordCountRatio)
            {
                for (auto ratio : settings.probabilityRatio)
                {
                    for (int i = 0; i < settings.repeat; ++i)
                    {
                        auto db = RandomValuesGenerator::GenerateRandomDB(1, settings.recordLength[0]);
                        NDB_QHiddenGenerator generator(*db.begin(), length, ratio, recordCount, definedBits);
                        DummyStream stream;
                        std::clock_t c_start = std::clock();
                        generator.Generate(stream);
                        std::clock_t c_end = std::clock();
                        auto solveTime = (c_end - c_start) / (CLOCKS_PER_SEC / 1000);
                        file << generator.GetCsvData().GetCsvRow() << ";" << solveTime << std::endl;
                    }
                }
            }
        }
    }

}

void GenerationTimeTest::TestKHidden(std::ofstream &file)
{
    auto settings = CLIInterface::GetSettings();
    file << FileUtils::CsvFileData::GetCsvDataHeader() << ";gen_time" << std::endl;
    for (auto length : settings.recordLength)
    {
        for (auto definedBits : settings.specifiedBits)
        {
            for (auto recordCount : settings.recordCountRatio)
            {
                for (const auto& ratio : CLIInterface::GetPVectors())
                {
                    for (int i = 0; i < settings.repeat; ++i)
                    {
                        auto db = RandomValuesGenerator::GenerateRandomDB(1, settings.recordLength[0]);
                        NDB_KHiddenGenerator generator(*db.begin(), length, ratio, recordCount, definedBits);
                        DummyStream stream;
                        std::clock_t c_start = std::clock();
                        generator.Generate(stream);
                        std::clock_t c_end = std::clock();
                        auto solveTime = (c_end - c_start) / (CLOCKS_PER_SEC / 1000);
                        file << generator.GetCsvData().GetCsvRow() << ";" << solveTime << std::endl;
                    }
                }
            }
        }
    }
}

void GenerationTimeTest::TestHybrid(std::ofstream &file)
{
    auto settings = CLIInterface::GetSettings();
    file << FileUtils::CsvFileData::GetCsvDataHeader() << ";gen_time" << std::endl;
    for (auto length : settings.recordLength)
    {
        for (auto recordCount : settings.recordCountRatio)
        {
            for (const auto& ratio : settings.probabilityRatio)
            {
                for (int i = 0; i < settings.repeat; ++i)
                {
                    auto db = RandomValuesGenerator::GenerateRandomDB(1, settings.recordLength[0]);
                    NDB_HybridGenerator generator(*db.begin(), length, ratio, recordCount);
                    DummyStream stream;
                    std::clock_t c_start = std::clock();
                    generator.Generate(stream);
                    std::clock_t c_end = std::clock();
                    auto solveTime = (c_end - c_start) / (CLOCKS_PER_SEC / 1000);
                    file << generator.GetCsvData().GetCsvRow() << ";" << solveTime << std::endl;
                }
            }
        }
    }
}

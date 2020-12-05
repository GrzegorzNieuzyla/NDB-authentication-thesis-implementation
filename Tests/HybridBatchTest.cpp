#include "HybridBatchTest.h"
#include "../Generators/NDB_HybridGenerator.h"

#include <utility>

void HybridBatchTest::Run(const std::string &filename)
{
    std::ofstream file(boost::ends_with(filename, ".csv") ? filename : filename + ".csv");
    file << FileUtils::CsvFileData::GetCsvDataHeader() << ";gen_time;solved;time;" << (_solver == GeneratorBatchTest::Solver::ZChaff ? "decisions" : "flips") << std::endl;
    for (auto length : _lengths)
    {
        for (auto recordCount : _recordCountRatios)
        {
            for (auto ratio : _probabilityRatios)
            {
                for (int i = 0; i < _repeat; ++i)
                {
                    RunTest(file, length, ratio, recordCount);
                }
            }
        }
    }
}

HybridBatchTest::HybridBatchTest(GeneratorBatchTest::Solver solver, std::vector<int> lengths,
                                 std::vector<double> probabilityRatios,
                                 std::vector<double> recordCountRatios, int repeat) : GeneratorBatchTest(solver),
                                                                                             _lengths(std::move(lengths)),
                                                                                             _probabilityRatios(std::move(
                                                                                                     probabilityRatios)),
                                                                                             _recordCountRatios(std::move(
                                                                                                     recordCountRatios)),
                                                                                             _repeat(repeat)
{}

void HybridBatchTest::RunTest(std::ofstream &file, int length, double ratio, double count)
{
    auto db = RandomValuesGenerator::GenerateRandomDB(1, length);
    NDB_HybridGenerator generator(*db.begin(), length, ratio, count);
    file << generator.GetCsvData().GetCsvRow();
    if (_solver == Solver::ZChaff)
    {
        auto result = ZChaffSolveTest(generator).Solve();
        file << ";" << result.genTime << ";" << result.solved << ";" << result.solveTime << ";" << result.decisions;
    }
    else if (_solver == Solver::WalkSat)
    {
        auto result = WalkSatSolveTest(generator).Solve();
        file << ";" << result.genTime << ";" << result.solved << ";" << result.solveTime << ";" << result.flips;
    }
    file << std::endl;
}

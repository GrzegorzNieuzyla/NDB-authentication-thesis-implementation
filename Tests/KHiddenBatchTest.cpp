#include "KHiddenBatchTest.h"
#include "../Generators/NDB_KHiddenGenerator.h"
#include "../CLIInterface.h"

#include <utility>

void KHiddenBatchTest::Run(const std::string &filename)
{
    std::ofstream file(boost::ends_with(filename, ".csv") ? filename : filename + ".csv");
    file << FileUtils::CsvFileData::GetCsvDataHeader() << ";gen_time;solved;time;" << (_solver == GeneratorBatchTest::Solver::ZChaff ? "decisions" : "flips") << std::endl;
    auto settings = CLIInterface::GetSettings();
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
                        RunTest(file, length, ratio, recordCount, definedBits);
                    }
                }
            }
        }
    }
}

KHiddenBatchTest::KHiddenBatchTest(GeneratorBatchTest::Solver solver) : GeneratorBatchTest(solver)
{}

void KHiddenBatchTest::RunTest(std::ofstream &file, int length, const std::vector<double> &prob, double count, int bits)
{
    auto db = RandomValuesGenerator::GenerateRandomDB(1, length);
    NDB_KHiddenGenerator generator(*db.begin(), length, prob, count, bits);
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

#include "QHiddenBatchTest.h"
#include "../Generators/NDB_QHiddenGenerator.h"
#include "../CLIInterface.h"

#include <utility>

void QHiddenBatchTest::Run(const std::string &filename)
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
                for (auto ratio : settings.probabilityRatio)
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

QHiddenBatchTest::QHiddenBatchTest(GeneratorBatchTest::Solver solver) : GeneratorBatchTest(solver)
{
}

void QHiddenBatchTest::RunTest(std::ofstream &file, int length, double ratio, double recordCount, int bits)
{
    auto db = RandomValuesGenerator::GenerateRandomDB(1, length);
    NDB_QHiddenGenerator generator(*db.begin(), length, ratio, recordCount, bits);
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

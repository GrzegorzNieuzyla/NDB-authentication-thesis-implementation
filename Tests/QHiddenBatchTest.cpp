#include "QHiddenBatchTest.h"
#include "../Generators/NDB_QHiddenGenerator.h"

#include <utility>

void QHiddenBatchTest::Run(const std::string &filename)
{
    std::ofstream file(boost::ends_with(filename, ".csv") ? filename : filename + ".csv");
    file << FileUtils::CsvFileData::GetCsvDataHeader() << ";solved;time;" << (_solver == GeneratorBatchTest::Solver::ZChaff ? "decisions" : "flips") << std::endl;
    for (auto length : _lengths)
    {
        for (auto definedBits : _definedBits)
        {
            for (auto recordCount : _recordCountRatios)
            {
                for (auto ratio : _probabilityRatios)
                {
                    for (int i = 0; i < _repeat; ++i)
                    {
                        RunTest(file, length, ratio, recordCount, definedBits);
                    }
                }
            }
        }
    }
}

QHiddenBatchTest::QHiddenBatchTest(GeneratorBatchTest::Solver solver, std::vector<int> lengths,
                                   std::vector<double> probabilityRatios, std::vector<double> recordCountRatios, std::vector<int> definedBits,
                                   int repeat) : GeneratorBatchTest(solver), _lengths(std::move(lengths)), _recordCountRatios(std::move(recordCountRatios)),
                                                 _probabilityRatios(std::move(probabilityRatios)), _definedBits(std::move(definedBits)),
                                                 _repeat(repeat)
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
        file << ";" << result.solved << ";" << result.seconds << ";" << result.decisions;
    }
    else if (_solver == Solver::WalkSat)
    {
        auto result = WalkSatSolveTest(generator).Solve();
        file << ";" << result.solved << ";" << result.seconds << ";" << result.flips;
    }
    file << std::endl;
}

#include "PrefixBatchTest.h"
#include "../Generators/NDB_PrefixGenerator.h"

PrefixBatchTest::PrefixBatchTest(Solver solver, std::vector<int> counts, std::vector<int> lengths, int repeat)
        : GeneratorBatchTest(solver), _counts(std::move(counts)),
          _lengths(std::move(lengths)), _repeat(repeat)
{

}

void PrefixBatchTest::Run(const std::string &filename)
{
    std::ofstream file(boost::ends_with(filename, ".csv") ? filename : filename + ".csv");
    file << FileUtils::CsvFileData::GetCsvDataHeader() << ";solved;time;" << (_solver == GeneratorBatchTest::Solver::ZChaff ? "decisions" : "flips") << std::endl;
    for (auto length : _lengths)
    {
        for (auto count : _counts)
        {
            for (int i = 0; i < _repeat; ++i)
            {
                RunTest(file, count, length);
            }
        }
    }
}

void PrefixBatchTest::RunTest(std::ofstream& file, int count, int length)
{
    auto db = RandomValuesGenerator::GenerateRandomDB(count, length);
    NDB_PrefixGenerator generator(db, length);
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
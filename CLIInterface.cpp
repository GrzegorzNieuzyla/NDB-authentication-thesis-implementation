#include "CLIInterface.h"
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <iomanip>
#include "Generators/NDB_PrefixGenerator.h"
#include "Generators/NDB_QHiddenGenerator.h"
#include "Generators/NDB_KHiddenGenerator.h"
#include "Generators/NDB_RandomizedGenerator.h"
#include "Generators/NDB_RandomizedOldGenerator.h"
#include "Generators/NDB_HybridGenerator.h"
#include "Generators/NDB_0HiddenGenerator.h"
#include "Streams/DimacsFileStream.h"
#include "Streams/NDBFileStream.h"
#include "Utils/Benchmark.h"
#include "Tests/SuperfluousStringTest.h"
#include "Tests/DistributionTest.h"
#include "Streams/NDBStream.h"


namespace po = boost::program_options;
namespace alg = boost::algorithm;

CLIInterface::CLIInterface(int argc, char* argv[])
{
    SetupCommandLine(argc, argv);
}

void CLIInterface::SetupGenerator(const DB& db)
{
    if (_algorithm == alg::to_lower_copy(NDB_PrefixGenerator::GetName()))
    {
        _generator = std::unique_ptr<NDB_Generator>(new NDB_PrefixGenerator(db, _recordLength));
    }
    else if (_algorithm == alg::to_lower_copy(NDB_RandomizedOldGenerator::GetName()))
    {
        _generator = std::unique_ptr<NDB_Generator>(new NDB_RandomizedOldGenerator(db, _recordLength));
    }
    else if (_algorithm == alg::to_lower_copy(NDB_RandomizedGenerator::GetName()))
    {
        _generator = std::unique_ptr<NDB_Generator>(new NDB_RandomizedGenerator(db, _recordLength));
    }
    else if (_algorithm == alg::to_lower_copy(NDB_0HiddenGenerator::GetName()))
    {
        _generator = std::unique_ptr<NDB_Generator>(new NDB_0HiddenGenerator(_recordLength, _recordCountRatio, _specifiedBits));
    }
    else if (_algorithm == alg::to_lower_copy(NDB_QHiddenGenerator::GetName()))
    {
        _generator = std::unique_ptr<NDB_Generator>(new NDB_QHiddenGenerator(*db.begin(), _recordLength, _probabilityRatio, _recordCountRatio, _specifiedBits));
    }
    else if (_algorithm == alg::to_lower_copy(NDB_KHiddenGenerator::GetName()))
    {
        _generator = std::unique_ptr<NDB_Generator>(new NDB_KHiddenGenerator(*db.begin(), _recordLength, _probabilityRatios, _recordCountRatio, _specifiedBits));
    }
    else if (_algorithm == alg::to_lower_copy(NDB_HybridGenerator::GetName()))
    {
        _generator = std::unique_ptr<NDB_Generator>(new NDB_HybridGenerator(*db.begin(), _recordLength, _probabilityRatio, _recordCountRatio));
    }
    else
    {
        std::cerr << "No such algorithm: " << _algorithm << std::endl;
        exit(1);
    }
}

void CLIInterface::SetupCommandLine(int argc, char* argv[])
{

    try{
        std::vector<std::string> availableGenerators = {
                NDB_PrefixGenerator::GetName(), NDB_RandomizedGenerator::GetName(),
                NDB_RandomizedOldGenerator::GetName(), NDB_0HiddenGenerator::GetName(),
                NDB_QHiddenGenerator::GetName(), NDB_KHiddenGenerator::GetName(),
                NDB_HybridGenerator::GetName()
        };
        auto algorithms = "Set algorithm (" + alg::join(availableGenerators, " | ") + ")";
        _description.add_options()
                ("record-count-ratio,r", po::value<double>(&_recordCountRatio), "Record count parameter")
                ("probability-ratio,q", po::value<double>(&_probabilityRatio), "Probability parameter")
                ("probability-ratios,p", po::value<std::vector<double>>(&_probabilityRatios)->multitoken(), "Probability parameters")
                ("record-length,l", po::value<int>(&_recordLength), "Record length")
                ("record-count,c", po::value<int>(&_recordCount), "Record count")
                ("specified-bits,k", po::value<int>(&_specifiedBits), "Specified bits count")
                ("output-file,o", po::value<std::string>(&_outputFile), "Output file")
                ("format,f", po::value<std::string>(&_generationMethod), "Output format (dimacs | ndb)")
                ("superfluous,s", "Don't generate file, check for superfluous strings")
                ("checksum-bits,cb", po::value<int>(&_checksumBits), "Checksum bits count")
                ("db-record", po::value<std::string>(&_dbRecord), "Specify DB record")
                ("distribution", "Run distribution test")
                ("help,h", "Produce help message")
                ("algorithm,a", po::value<std::string>(&_algorithm)->required(), algorithms.c_str());

        po::store(po::parse_command_line(argc, argv, _description), _variablesMap);

        if (_variablesMap.count("help"))
        {
            std::cout << _description << "\n";
            exit(0);
        }
        po::notify(_variablesMap);

        if (_variablesMap.count("probability-ratios"))
        {
            _probabilityRatios = _variablesMap["probability-ratios"].as<std::vector<double>>();
        }

        if (_specifiedBits <= 0 || _recordLength <= 0 || _probabilityRatio < 0 || _probabilityRatio > 1 || _recordCountRatio <= 0)
        {
            std::cerr << "Incorrect parameter value" << std::endl;
            exit(1);
        }
        _superfluousStringTesting = _variablesMap.count("superfluous");
        _distributionTesting = _variablesMap.count("distribution");
        if (!_dbRecord.empty())
            _recordLength = _dbRecord.size();
        alg::to_lower(_algorithm);
    }
    catch (po::required_option& e)
    {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
    catch(...) {
        std::cout << _description << "\n";
        exit(0);
    }

}

void CLIInterface::Run()
{
    if (_superfluousStringTesting)
    {
        RunSuperfluousStringsTest();
    }
    else if (_distributionTesting)
    {
        RunDistributionTest();
    }
    else
    {
        RunGenerator();
    }
}

std::string CLIInterface::GetTimeElapsed(std::size_t seconds)
{
    if (seconds < 60) return std::to_string(seconds) + "s";
    if (seconds < 3600) return std::to_string(seconds / 60) + "m" + std::to_string(seconds % 60) + "s";
    return std::to_string(seconds / 3600) + "h" + std::to_string((seconds % 3600) / 60) + "m" + std::to_string(seconds % 60) + "s";
}

void CLIInterface::RunSuperfluousStringsTest()
{
    if (_algorithm == alg::to_lower_copy(NDB_QHiddenGenerator::GetName()))
    {
        SuperfluousStringTest test(SuperfluousStringTest::GetChecksumType(_checksumBits), _recordLength, _probabilityRatio, _recordCountRatio, _specifiedBits);
        test.Run();
    }
    else if (_algorithm == alg::to_lower_copy(NDB_KHiddenGenerator::GetName()))
    {
        SuperfluousStringTest test(SuperfluousStringTest::GetChecksumType(_checksumBits), _recordLength, _probabilityRatios, _recordCountRatio, _specifiedBits);
        test.Run();
    }
}

void CLIInterface::RunGenerator()
{
    if (!_dbRecord.empty())
    {
        SetupGenerator(DB {DBRecord::FromString(_dbRecord)});
    }
    else
    {
        SetupGenerator(RandomValuesGenerator().GenerateRandomDB(_recordCount, _recordLength));
    }
    _generator->PrintParameters();
    std::unique_ptr<Stream> stream;
    alg::to_lower(_generationMethod);
    if (_outputFile.empty())
    {
        _outputFile = _algorithm + "_" +
                      (_algorithm == NDB_PrefixGenerator::GetName() || _algorithm == NDB_RandomizedOldGenerator::GetName() ||
                       _algorithm == NDB_RandomizedGenerator::GetName() ? std::to_string(_recordCount) + "_" : "") +
                      std::to_string(_recordLength);
    }


    if (_generationMethod == "dimacs")
    {
        if (!alg::ends_with(_outputFile, ".dimacs")) _outputFile += ".dimacs";
        std::cout << "Format: Dimacs" << std::endl;
        std::cout << "Output: " << _outputFile << std::endl;
        stream = std::unique_ptr<Stream>(new DimacsFileStream(_outputFile));
    }
    else if (_generationMethod == "ndb")
    {
        if (!alg::ends_with(_outputFile, ".ndb")) _outputFile += ".ndb";
        std::cout << "Format: NDB" << std::endl;
        std::cout << "Output: " << _outputFile << std::endl;
        stream = std::unique_ptr<Stream>(new NDBFileStream(_outputFile));
    }
    else
    {
        std::cerr << "Unknown format: " << _generationMethod << std::endl;
        exit(1);
    }

    Benchmark benchmark;
    benchmark.Start();
    auto count = _generator->Generate(*stream);
    auto elapsed_time = benchmark.GetElapsedTime();
    std::cout << "Generated records: " << count << std::endl;
    std::cout << GetTimeElapsed(elapsed_time) << std::endl;

    if (_generationMethod == "dimacs")
    {
        dynamic_cast<DimacsFileStream*>(stream.get())->WriteHeader(_recordLength, count);
    }
}

void CLIInterface::RunDistributionTest()
{
    if (!_dbRecord.empty())
    {
        SetupGenerator(DB {DBRecord::FromString(_dbRecord)});
    }
    else
    {
        SetupGenerator(RandomValuesGenerator().GenerateRandomDB(1, _recordLength));
    }
    NDBStream ndb;
    _generator->Generate(ndb);
    auto result = DistributionTest::CalculateProbability(ndb.Ndb());
    std::cout << "\t";
    for (int i = 1; i <= _recordLength; ++i)
        std::cout << i << '\t';
    std::cout << std::endl;

    for (auto ch : {NDBChar::Bit0, NDBChar::Bit1, NDBChar::Wildcard})
    {
        std::cout << (ch == NDBChar::Bit0 ? '0' : (ch == NDBChar::Bit1 ? '1' : '*')) << ":\t";
        for (auto i : result[ch])
            std::cout << std::setprecision(3) << i << '\t';
        std::cout << std::accumulate(result[ch].begin(), result[ch].end(), 0.0)  / result[ch].size();
        std::cout << std::endl;
    }
    std::cout << std::endl;
    auto resultDefined = DistributionTest::CalculateProbabilityForDefined(ndb.Ndb());

    for (auto ch : {NDBChar::Bit0, NDBChar::Bit1})
    {
        std::cout << (ch == NDBChar::Bit0 ? '0' : '1') << ":\t";
        for (auto i : resultDefined[ch])
            std::cout << std::setprecision(3) << i << '\t';
        std::cout << std::accumulate(resultDefined[ch].begin(), resultDefined[ch].end(), 0.0) / resultDefined[ch].size();
        std::cout << std::endl;
    }
    std::cout << std::endl;

    auto entropy = DistributionTest::CalculateEntropy(ndb.Ndb());
    auto gini = DistributionTest::CalculateGini(ndb.Ndb());
    std::cout << "Ent:\t";
    for (auto val : entropy)
    {
        std::cout << val << '\t';
    }
    std::cout << std::accumulate(entropy.begin(), entropy.end(), 0.0) / entropy.size();
    std::cout << std::endl;

    std::cout << "Gin:\t";

    for (auto val : gini)
    {
        std::cout << val << '\t';
    }
    std::cout << std::accumulate(gini.begin(), gini.end(), 0.0) / gini.size();
    std::cout << std::endl;
    std::cout << std::endl;

    entropy = DistributionTest::CalculateEntropyForDefined(ndb.Ndb());
    gini = DistributionTest::CalculateGiniForDefined(ndb.Ndb());
    std::cout << "Ent:\t";
    for (auto val : entropy)
    {
        std::cout << val << '\t';
    }
    std::cout << std::accumulate(entropy.begin(), entropy.end(), 0.0) / entropy.size();
    std::cout << std::endl;

    std::cout << "Gin:\t";

    for (auto val : gini)
    {
        std::cout << val << '\t';
    }
    std::cout << std::accumulate(gini.begin(), gini.end(), 0.0) / gini.size();
    std::cout << std::endl;
}

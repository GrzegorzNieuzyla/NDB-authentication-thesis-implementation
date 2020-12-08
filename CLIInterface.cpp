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
#include "Tests/SuperfluousStringBatchTest.h"
#include "Tests/PrefixBatchTest.h"
#include "Tests/RandomizerBatchTest.h"
#include "Tests/QHiddenBatchTest.h"
#include "Tests/KHiddenBatchTest.h"
#include "Tests/HybridBatchTest.h"
#include "Tests/GenerationTimeTest.h"
#include "Tests/DistributionBatchTest.h"


namespace po = boost::program_options;
namespace alg = boost::algorithm;

CLIInterface::CLIInterface(int argc, char* argv[])
{
    SetupCommandLine(argc, argv);
}

void CLIInterface::SetupGenerator(const DB& db)
{
    if (_settings.algorithm == alg::to_lower_copy(NDB_PrefixGenerator::GetName()))
    {
        _generator = std::unique_ptr<NDB_Generator>(new NDB_PrefixGenerator(db, GetSettings().recordLength[0]));
    }
    else if (_settings.algorithm == alg::to_lower_copy(NDB_RandomizedOldGenerator::GetName()))
    {
        _generator = std::unique_ptr<NDB_Generator>(new NDB_RandomizedOldGenerator(db, GetSettings().recordLength[0]));
    }
    else if (_settings.algorithm == alg::to_lower_copy(NDB_RandomizedGenerator::GetName()))
    {
        _generator = std::unique_ptr<NDB_Generator>(new NDB_RandomizedGenerator(db, GetSettings().recordLength[0]));
    }
    else if (_settings.algorithm == alg::to_lower_copy(NDB_0HiddenGenerator::GetName()))
    {
        _generator = std::unique_ptr<NDB_Generator>(new NDB_0HiddenGenerator(GetSettings().recordLength[0], GetSettings().recordCountRatio[0], GetSettings().specifiedBits[0]));
    }
    else if (_settings.algorithm == alg::to_lower_copy(NDB_QHiddenGenerator::GetName()))
    {
        _generator = std::unique_ptr<NDB_Generator>(new NDB_QHiddenGenerator(*db.begin(), GetSettings().recordLength[0], GetSettings().probabilityRatio[0], GetSettings().recordCountRatio[0], GetSettings().specifiedBits[0]));
    }
    else if (_settings.algorithm == alg::to_lower_copy(NDB_KHiddenGenerator::GetName()))
    {
        _generator = std::unique_ptr<NDB_Generator>(new NDB_KHiddenGenerator(*db.begin(), GetSettings().recordLength[0], GetSettings().probabilityRatios, GetSettings().recordCountRatio[0], GetSettings().specifiedBits[0]));
    }
    else if (_settings.algorithm == alg::to_lower_copy(NDB_HybridGenerator::GetName()))
    {
        _generator = std::unique_ptr<NDB_Generator>(new NDB_HybridGenerator(*db.begin(), GetSettings().recordLength[0], GetSettings().probabilityRatio[0], GetSettings().recordCountRatio[0]));
    }
    else
    {
        std::cerr << "No such algorithm: " << _settings.algorithm << std::endl;
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
                ("record-count-ratio,r", po::value<std::vector<double>>(&_settings.recordCountRatio)->multitoken(), "Record count parameter")
                ("probability-ratio,q", po::value<std::vector<double>>(&_settings.probabilityRatio)->multitoken(), "Probability parameter")
                ("probability-ratios,p", po::value<std::vector<double>>(&_settings.probabilityRatios)->multitoken(), "Probability parameters")
                ("record-length,l", po::value<std::vector<int>>(&_settings.recordLength)->multitoken(), "Record length")
                ("record-count,c", po::value<std::vector<int>>(&_settings.recordCount)->multitoken(), "Record count")
                ("specified-bits,k", po::value<std::vector<int>>(&_settings.specifiedBits)->multitoken(), "Specified bits count")
                ("output-file,o", po::value<std::string>(&_outputFile), "Output file")
                ("format,f", po::value<std::string>(&_generationMethod), "Output format (dimacs | ndb)")
                ("superfluous,s", "Don't generate file, check for superfluous strings")
                ("batch", "Enable batch mode")
                ("repeat", po::value<int>(&_settings.repeat), "Specify times to repeat test")
                ("checksum-bits", po::value<std::vector<int>>(&_settings.checksumBits)->multitoken(), "Checksum bits count")
                ("db-record", po::value<std::string>(&_dbRecord), "Specify DB record")
                ("distribution", "Run distribution test")
                ("help,h", "Produce help message")
                ("solve-tests", "Run solve tests")
                ("gen-time-tests", "Run generation time tests")
                ("cutoff", po::value<std::size_t>(&_settings.walksatCutoffLimit), "Specify WalkSAT cutoff")
                ("time-limit", po::value<std::size_t>(&_settings.zchaffTimeLimit), "Specify zChaff time limit")
                ("solver", po::value<std::string>(&_solver), "Specify solver (zchaff | walksat)")
                ("algorithm,a", po::value<std::string>(&_settings.algorithm)->required(), algorithms.c_str());

        po::store(po::parse_command_line(argc, argv, _description), _variablesMap);

        if (_variablesMap.count("help"))
        {
            std::cout << _description << "\n";
            exit(0);
        }
        po::notify(_variablesMap);

        if (_variablesMap.count("probability-ratios"))
        {
            _settings.probabilityRatios = _variablesMap["probability-ratios"].as<std::vector<double>>();
        }

        if (_variablesMap.count("probability-ratio"))
        {
            _settings.probabilityRatio = _variablesMap["probability-ratio"].as<std::vector<double>>();
        }
        if (_variablesMap.count("record-length"))
        {
            _settings.recordLength = _variablesMap["record-length"].as<std::vector<int>>();
        }

        if (_variablesMap.count("specified-bits"))
        {
            _settings.specifiedBits = _variablesMap["specified-bits"].as<std::vector<int>>();
        }

        if (_variablesMap.count("record-count"))
        {
            _settings.recordCount = _variablesMap["record-count"].as<std::vector<int>>();
        }
        if (_variablesMap.count("record-count-ratio"))
        {
            _settings.recordCountRatio = _variablesMap["record-count-ratio"].as<std::vector<double>>();
        }
        if (_variablesMap.count("checksum-bits"))
        {
            _settings.checksumBits = _variablesMap["checksum-bits"].as<std::vector<int>>();
        }

        if (_settings.specifiedBits[0] <= 0 || _settings.recordLength[0] <= 0 || _settings.probabilityRatio[0] < 0 || _settings.probabilityRatio[0] > 1 || _settings.recordCountRatio[0] <= 0)
        {
            std::cerr << "Incorrect parameter value" << std::endl;
            exit(1);
        }
        _superfluousStringTesting = _variablesMap.count("superfluous");
        _solveTesting = _variablesMap.count("solve-tests");
        _distributionTesting = _variablesMap.count("distribution");
        _batchMode = _variablesMap.count("batch");
        _genTimeTest = _variablesMap.count("gen-time-tests");
        if (!_dbRecord.empty())
            _settings.recordLength[0] = _dbRecord.size();
        alg::to_lower(_settings.algorithm);
        alg::to_lower(_solver);
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
    else if (_solveTesting)
    {
        RunSolveTests();
    }
    else if(_genTimeTest)
    {
        RunGenerationTimeTests();
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
    if (_batchMode)
    {
        if (_outputFile.empty())
        {
            std::cerr << "Output file not provided\n";
            exit(1);
        }
        std::vector<Checksum::ChecksumType> checksums;
        std::transform(_settings.checksumBits.begin(), _settings.checksumBits.end(), std::back_inserter(checksums), Checksum::GetChecksumType);
        if (_settings.algorithm == alg::to_lower_copy(NDB_QHiddenGenerator::GetName()))
        {
            SuperfluousStringBatchTest(_settings.recordLength, _settings.recordCountRatio, _settings.probabilityRatio[0], _settings.specifiedBits[0], checksums, _settings.repeat).Run(_outputFile);
        }
        else if (_settings.algorithm == alg::to_lower_copy(NDB_KHiddenGenerator::GetName()))
        {
            SuperfluousStringBatchTest(_settings.recordLength, _settings.recordCountRatio, _settings.probabilityRatios, _settings.specifiedBits[0], checksums, _settings.repeat).Run(_outputFile);
        }
        return;
    }

    if (_settings.algorithm == alg::to_lower_copy(NDB_QHiddenGenerator::GetName()))
    {
        SuperfluousStringTest test(Checksum::GetChecksumType(_settings.checksumBits[0]), _settings.recordLength[0], _settings.probabilityRatio[0], _settings.recordCountRatio[0], _settings.specifiedBits[0]);
        test.Run();
    }
    else if (_settings.algorithm == alg::to_lower_copy(NDB_KHiddenGenerator::GetName()))
    {
        SuperfluousStringTest test(Checksum::GetChecksumType(_settings.checksumBits[0]), _settings.recordLength[0], _settings.probabilityRatios[0], _settings.recordCountRatio[0], _settings.specifiedBits[0]);
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
        SetupGenerator(RandomValuesGenerator::GenerateRandomDB(_settings.recordCount[0], _settings.recordLength[0]));
    }
    _generator->PrintParameters();
    std::unique_ptr<Stream> stream;
    alg::to_lower(_generationMethod);
    if (_outputFile.empty())
    {
        _outputFile = _settings.algorithm + "_" +
                      (_settings.algorithm == NDB_PrefixGenerator::GetName() || _settings.algorithm == NDB_RandomizedOldGenerator::GetName() ||
                       _settings.algorithm == NDB_RandomizedGenerator::GetName() ? std::to_string(_settings.recordCount[0]) + "_" : "") +
                      std::to_string(_settings.recordLength[0]);
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
        dynamic_cast<DimacsFileStream*>(stream.get())->WriteHeader(_settings.recordLength[0], count);
    }
}

void CLIInterface::RunDistributionTest()
{
    DistributionBatchTest::Run();
}

std::vector<std::vector<double>> CLIInterface::GetPVectors()
{
    std::vector<std::vector<double>> result;
    result.emplace_back();
    for (auto r : _settings.probabilityRatios)
    {
        if (result.back().size() == _settings.specifiedBits[0])
            result.emplace_back();
        result.back().push_back(r);
    }

    return result;
}

void CLIInterface::RunSolveTests()
{

    GeneratorBatchTest::Solver solver;
    if (_solver == "zchaff")
    {
        solver = GeneratorBatchTest::Solver::ZChaff;
    }
    else if (_solver == "walksat")
    {
        solver = GeneratorBatchTest::Solver::WalkSat;
    }
    else
    {
        std::cerr << "Invalid solver: " << _solver << std::endl;
        exit(1);
    }
    if (_outputFile.empty())
    {
        std::cerr << "Output file not specified " << std::endl;
        exit(1);
    }
    if (_settings.algorithm == alg::to_lower_copy(NDB_PrefixGenerator::GetName()))
    {
        PrefixBatchTest(solver).Run(_outputFile);
    }
    else if (_settings.algorithm == alg::to_lower_copy(NDB_RandomizedGenerator::GetName()))
    {
        RandomizerBatchTest(solver).Run(_outputFile);
    }
    else if (_settings.algorithm == alg::to_lower_copy(NDB_QHiddenGenerator::GetName()))
    {
        QHiddenBatchTest(solver).Run(_outputFile);
    }
    else if (_settings.algorithm == alg::to_lower_copy(NDB_KHiddenGenerator::GetName()))
    {
        KHiddenBatchTest(solver).Run(_outputFile);
    }
    else if (_settings.algorithm == alg::to_lower_copy(NDB_HybridGenerator::GetName()))
    {
        HybridBatchTest(solver).Run(_outputFile);
    }
    else
    {
        std::cerr << "No such algorithm: " << _settings.algorithm << std::endl;
        exit(1);
    }
}

const CLIInterface::Settings &CLIInterface::GetSettings()
{
    return _settings;
}

void CLIInterface::RunGenerationTimeTests()
{
    if (_outputFile.empty())
    {
        std::cerr << "Output file not provided\n";
        exit(1);
    }
    GenerationTimeTest().RunTests(_outputFile);
}

CLIInterface::Settings CLIInterface::_settings;
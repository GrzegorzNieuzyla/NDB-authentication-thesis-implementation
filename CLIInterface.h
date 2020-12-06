#pragma once
#include <vector>
#include <string>
#include "Generators/NDB_Generator.h"
#include <boost/program_options.hpp>

class CLIInterface
{
public:
    CLIInterface(int argc, char* argv[]);
    void Run();
    struct Settings
    {
        std::size_t zchaffTimeLimit = 3600 * 24;
        std::size_t walksatCutoffLimit = 1000000000;
        std::vector<int> recordLength = {256};
        std::vector<double> probabilityRatio = {0.5};
        std::vector<double> recordCountRatio = {5.5};
        std::vector<int> specifiedBits = {3};
        std::vector<int> recordCount = {1};
        std::vector<double> probabilityRatios = {0.25, 0.5, 0.25};
        std::vector<int> checksumBits = {0};
        int repeat = 1;
    };
    static const Settings& GetSettings();
    [[nodiscard]] static std::vector<std::vector<double>> GetPVectors();

private:
    void SetupCommandLine(int argc, char* argv[]);
    void SetupGenerator(const DB& db);
    void RunGenerator();
    void RunSuperfluousStringsTest();
    void RunDistributionTest();
    void RunSolveTests();
    void RunGenerationTimeTests();
    static std::string GetTimeElapsed(std::size_t seconds);
    std::unique_ptr<NDB_Generator> _generator;
    std::vector<std::string> _arguments;
    boost::program_options::variables_map _variablesMap;
    boost::program_options::options_description _description {"Allowed commands"};

    std::string _algorithm;


    std::string _outputFile;
    std::string _generationMethod = "dimacs";

    bool _superfluousStringTesting = false;

    std::string _dbRecord;
    bool _distributionTesting = false;

    bool _genTimeTest = false;
    bool _batchMode = false;
    bool _solveTesting = false;
    std::string _solver = "zchaff";
    static Settings _settings;
};

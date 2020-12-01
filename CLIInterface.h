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
private:
    void SetupCommandLine(int argc, char* argv[]);
    void SetupGenerator(const DB& db);
    void RunGenerator();
    void RunSuperfluousStringsTest();
    void RunDistributionTest();
    void RunSolveTests();
    static std::string GetTimeElapsed(std::size_t seconds);
    [[nodiscard]] std::vector<std::vector<double>> GetPVectors() const;
    std::unique_ptr<NDB_Generator> _generator;
    std::vector<std::string> _arguments;
    boost::program_options::variables_map _variablesMap;
    boost::program_options::options_description _description {"Allowed commands"};

    std::string _algorithm;
    std::vector<int> _recordLength = {256};
    std::vector<double> _probabilityRatio = {0.5};
    std::vector<double> _recordCountRatio = {5.5};
    std::vector<int> _specifiedBits = {3};
    std::vector<int> _recordCount = {1};
    std::vector<double> _probabilityRatios = {0.25, 0.5, 0.25};

    std::string _outputFile;
    std::string _generationMethod = "dimacs";

    bool _superfluousStringTesting = false;
    std::vector<int> _checksumBits = {0};

    std::string _dbRecord;
    bool _distributionTesting = false;

    bool _batchMode = false;
    int  _repeat = 1;
    bool _solveTesting = false;
    std::string _solver = "zchaff";
};

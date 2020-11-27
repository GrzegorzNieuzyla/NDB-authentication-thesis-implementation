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
    void SetupGenerator();
    static std::string GetTimeElapsed(std::size_t seconds);
    std::unique_ptr<NDB_Generator> _generator;
    std::vector<std::string> _arguments;
    boost::program_options::variables_map _variablesMap;
    boost::program_options::options_description _description {"Allowed commands"};

    std::string _algorithm;
    int _recordLength = 256;
    double _probabilityRatio = 0.5;
    double _recordCountRatio = 5.5;
    int _specifiedBits = 3;
    int _recordCount = 1;
    std::vector<double> _probabilityRatios = {0.25, 0.5, 0.75};

    std::string _outputFile;
    std::string _generationMethod = "dimacs";

    bool _superfluousStringTesting = false;
    int _checksumBits = 1;
};

#include "SuperfluousStringBatchTest.h"
#include "../Utils/FileUtils.h"
#include "SuperfluousStringTest.h"
#include <boost/algorithm/string.hpp>
#include <utility>
#include <fstream>

SuperfluousStringBatchTest::SuperfluousStringBatchTest(std::vector<int> lengths,
                                                       std::vector<double> recordCountRatios,
                                                       double probabilityRatio, int specifiedBits,
                                                       std::vector<Checksum::ChecksumType> checksumTypes)
   : _lengths(std::move(lengths)), _recordCountRatios(std::move(recordCountRatios)), _probabilityRatio(probabilityRatio), _specifiedBits(
        specifiedBits), _checksumTypes(std::move(checksumTypes))
{
    _algorithm = Algorithm::QHidden;
}

SuperfluousStringBatchTest::SuperfluousStringBatchTest(std::vector<int> lengths,
                                                       std::vector<double> recordCountRatios,
                                                       std::vector<double> probabilityRatios, int specifiedBits,
                                                       std::vector<Checksum::ChecksumType> checksumTypes)
: _lengths(std::move(lengths)), _recordCountRatios(std::move(recordCountRatios)), _probabilityRatios(std::move(probabilityRatios)), _specifiedBits(specifiedBits), _checksumTypes(std::move(checksumTypes))
{
    _algorithm = Algorithm::KHidden;
}

void SuperfluousStringBatchTest::Run(const std::string& filename)
{
    std::ofstream file(boost::ends_with(filename, ".csv") ? filename : filename + ".csv");
    file << FileUtils::CsvFileData::GetCsvDataHeader() << ";matched;unmatched;hamming;checksum\n";
    for (auto checksum : _checksumTypes)
    {
        for (auto length : _lengths)
        {
            for (auto ratio : _recordCountRatios)
            {
                RunTestCase(file, length, ratio, checksum);
            }
        }
    }
    file.close();
}

void SuperfluousStringBatchTest::RunTestCase(std::ofstream &file, int length, double recordCountRatio, Checksum::ChecksumType checksum)
{
    SuperfluousStringTest::Results results;
    if (_algorithm == Algorithm::QHidden)
    {
        results = SuperfluousStringTest(checksum, length, _probabilityRatio, recordCountRatio, _specifiedBits).Run();
    }
    else if (_algorithm == Algorithm::KHidden)
    {
        results = SuperfluousStringTest(checksum, length, _probabilityRatios, recordCountRatio, _specifiedBits).Run();
    }
    file << results.csvData.GetCsvRow() << (boost::format(";%1%;%2%;%3%;%4%") % results.matched % results.unmatched % results.avgHammingDistance % Checksum::GetChecksumBits(checksum)).str() << std::endl;
}




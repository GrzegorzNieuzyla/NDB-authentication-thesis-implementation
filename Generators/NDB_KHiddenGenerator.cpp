#include "NDB_KHiddenGenerator.h"
#include <boost/algorithm/string.hpp>
#include <utility>

NDB_KHiddenGenerator::NDB_KHiddenGenerator(const DBRecord& record, int length, std::vector<double> probabilityRatios, double recordCountRatio, int definedPositionCount)
        : _probabilityRatios(std::move(probabilityRatios)), _recordCountRatio(recordCountRatio), _definedPositionCount(definedPositionCount)
{
    assert(std::abs(std::accumulate(_probabilityRatios.begin(), _probabilityRatios.end(), 0.0) -1) < 0.01);
    assert(_definedPositionCount == _probabilityRatios.size());
    double ratio = 0;
    for (int i = 0; i < _definedPositionCount - 1; ++i)
    {
        ratio += _probabilityRatios[i];
        _accumulatedProbabilityRatios.push_back(ratio);
    }
    _db.push_back(record);
    _length = length;
}

size_t NDB_KHiddenGenerator::Generate(Stream &output)
{
    assert(!_db.empty());
    std::size_t count = 0, n = ceil(_length * _recordCountRatio);
    const auto& dbRecord = _db[0];
    while (count < n)
    {
        int specifiedBitsCount = GetSpecifiedBitsCount(RandomValuesGenerator::GetRandomDouble(0, 1));
        auto indices = RandomValuesGenerator::GetRandomIndices(_length, _definedPositionCount);
        NDBRecord record{ std::vector<NDBChar>(_length, NDBChar::Wildcard) };
        for (auto index : indices)
        {
            record.Characters()[index] = dbRecord.Characters()[index] ? NDBChar::Bit1 : NDBChar::Bit0;
        }

        auto bits = RandomValuesGenerator::GetRandomChoice(indices, specifiedBitsCount);
        for (auto bit : bits)
        {
            record.Characters()[bit] = dbRecord.Characters()[bit] ? NDBChar::Bit0 : NDBChar::Bit1;
        }

        output << record.ToString() << "\n";
        ++count;
    }
    return count;
}

std::string NDB_KHiddenGenerator::GetName()
{
    return "K-Hidden";
}

int NDB_KHiddenGenerator::GetSpecifiedBitsCount(double rand)
{
    for (int i = 0; i < _accumulatedProbabilityRatios.size(); ++i)
    {
        if (_accumulatedProbabilityRatios[i] > rand)
            return i+1;
    }
    return static_cast<int>(_accumulatedProbabilityRatios.size()) + 1;
}

void NDB_KHiddenGenerator::PrintParameters() const
{
    std::cout << "Algorithm: " << GetName() << std::endl;
    std::cout << "l: " << _length << std::endl;
    std::vector<std::string> param;
    param.resize(_probabilityRatios.size());
    std::transform(_probabilityRatios.begin(), _probabilityRatios.end(), param.begin(), [](const auto& p){ return std::to_string(p);});
    std::cout << "p: " << boost::algorithm::join(param, " ") << std::endl;
    std::cout << "r: " << _recordCountRatio << std::endl;
    std::cout << "k: " << _definedPositionCount << std::endl;
}

FileUtils::CsvFileData NDB_KHiddenGenerator::GetCsvData() const
{
    return FileUtils::CsvFileData {.algorithm=GetName(), .length = _length, .probabilityRatios = _probabilityRatios, .recordCountRatio = _recordCountRatio, .specifiedBits=_definedPositionCount};
}

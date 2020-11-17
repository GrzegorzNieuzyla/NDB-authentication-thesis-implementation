#include "NDB_KHiddenGenerator.h"

#include <utility>

NDB_KHiddenGenerator::NDB_KHiddenGenerator(const DBRecord& record, int length, std::vector<double> probabilityRatios, double recordCountRatio, int definedPositionCount)
        : _probabilityRatios(std::move(probabilityRatios)), _recordCountRatio(recordCountRatio), _definedPositionCount(definedPositionCount)
{
    assert(std::all_of(_probabilityRatios.begin(), _probabilityRatios.end(), [](double ratio){ return ratio > 0 && ratio < 1; }));
    assert(std::is_sorted(_probabilityRatios.begin(), _probabilityRatios.end()));
    assert(_definedPositionCount == _probabilityRatios.size() + 1);
    _db.push_back(record);
    _length = length;
}

size_t NDB_KHiddenGenerator::Generate(Stream &output)
{
    assert(!_db.empty());
    std::size_t count = 0, n = ceil(_length * _recordCountRatio);
    const auto& dbRecord = _db[0];
    while (count != n)
    {
        int specifiedBitsCount = GetSpecifiedBitsCount(_random.GetRandomDouble(0, 1));

        auto indices = _random.GetRandomIndices(_length, _definedPositionCount);
        NDBRecord record{ std::vector<NDBChar>(_length, NDBChar::Wildcard) };
        for (auto index : indices)
        {
            record.Characters()[index] = dbRecord.Characters()[index] ? NDBChar::Bit1 : NDBChar::Bit0;
        }

        auto bits = _random.GetRandomChoice(indices, specifiedBitsCount);
        for (auto bit : bits)
        {
            record.Characters()[bit] = dbRecord.Characters()[bit] ? NDBChar::Bit0 : NDBChar::Bit1;
        }

        output << record.ToString() << "\n";
        ++count;

        std::cout << "\r" << count << "/" << n << "    ";
    }
    std::cout << std::endl;
    return count;
}

std::string NDB_KHiddenGenerator::GetName()
{
    return "K-Hidden";
}

int NDB_KHiddenGenerator::GetSpecifiedBitsCount(double rand)
{
    for (int i = 0; i < _probabilityRatios.size(); ++i)
    {
        if (_probabilityRatios[i] > rand)
            return i+1;
    }
    return static_cast<int>(_probabilityRatios.size()) + 1;
}

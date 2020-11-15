#include "NDB_QHiddenGenerator.h"


NDB_QHiddenGenerator::NDB_QHiddenGenerator(const DBRecord& record, int length, double probabilityRatio, double recordCountRatio, int definedPositionCount)
    : _probabilityRatio(probabilityRatio), _recordCountRatio(recordCountRatio), _definedPositionCount(definedPositionCount)
{
    _db.push_back(record);
    _length = length;
}

NDB_QHiddenGenerator::NDB_QHiddenGenerator(int length, double probabilityRatio, double recordCountRatio, int definedPositionCount)
    : _probabilityRatio(probabilityRatio), _recordCountRatio(recordCountRatio), _definedPositionCount(definedPositionCount)
{
    _length = length;
}

size_t NDB_QHiddenGenerator::Generate(Stream &output)
{
    if (_db.empty()) return Generate0Hidden(output);

    std::size_t count = 0, n = ceil(_length * _recordCountRatio);
    const auto& dbRecord = _db[0];
    while (count != n)
    {
        auto indices = _random.GetRandomIndices(_length, _definedPositionCount);
        NDBRecord record{ std::vector<NDBChar>(_length, NDBChar::Wildcard) };
        for (auto index : indices)
        {
            record.Characters()[index] = dbRecord.Characters()[index] ? NDBChar::Bit1 : NDBChar::Bit0;
        }

        bool changed = false;
        while (!changed)
        {
            for (auto index : indices)
            {
                if (_random.GetRandomDouble(0, 1) < _probabilityRatio)
                {
                    changed = true;
                    record.Characters()[index] = dbRecord.Characters()[index] ? NDBChar::Bit0 : NDBChar::Bit1;
                }
            }
        }

        output << record.ToString() << "\n";
        ++count;

        std::cout << "\r" << count << "/" << n;
    }
    std::cout << std::endl;
    return count;
}

size_t NDB_QHiddenGenerator::Generate0Hidden(Stream &output)
{
    std::size_t count = 0, n = ceil(_length * _recordCountRatio);

    while (count != n)
    {
        auto indices = _random.GetRandomIndices(_length, _definedPositionCount);
        NDBRecord record{ std::vector<NDBChar>(_length, NDBChar::Wildcard) };
        for (auto index : indices)
        {
            record.Characters()[index] = (_random.GetRandomInt(0, 1) == 1 ? NDBChar::Bit1 : NDBChar::Bit0);
        }
        output << record.ToString() << "\n";
        ++count;

        std::cout << count << "/" << n << std::endl;
    }

    return count;
}

std::string NDB_QHiddenGenerator::GetName()
{
    return "Q-Hidden";
}

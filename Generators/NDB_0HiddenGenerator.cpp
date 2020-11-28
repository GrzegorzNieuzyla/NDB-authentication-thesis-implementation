#include "NDB_0HiddenGenerator.h"

NDB_0HiddenGenerator::NDB_0HiddenGenerator(int length, double recordCountRatio, int definedPositionCount) : _recordCountRatio(
        recordCountRatio), _definedPositionCount(definedPositionCount)
{
    _length = length;
}

size_t NDB_0HiddenGenerator::Generate(Stream &output)
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

        std::cout << "\r" << count << "/" << n << "    ";
    }
    std::cout << std::endl;

    return count;
}

std::string NDB_0HiddenGenerator::GetName()
{
    return "0-Hidden";
}

void NDB_0HiddenGenerator::PrintParameters() const
{
    std::cout << "Algorithm: " << GetName() << std::endl;
    std::cout << "l: " << _length << std::endl;
    std::cout << "r: " << _recordCountRatio << std::endl;
    std::cout << "k: " << _definedPositionCount << std::endl;
}

FileUtils::CsvFileData NDB_0HiddenGenerator::GetCsvData() const
{
    return FileUtils::CsvFileData {.algorithm=GetName(), .length = _length, .recordCountRatio = _recordCountRatio, .specifiedBits = _definedPositionCount };
}

#include "NDB_HybridGenerator.h"

size_t NDB_HybridGenerator::Generate(Stream &output)
{
    assert(!_db.empty());
    auto permutation = _random.GenerateRandomPermutation(_length);
    auto ndb = GenComplete(permutation);
    for (const auto& record : ndb.Records())
    {
        output << record.ToString() << "\n";
    }

    return MakeHardReverse(ndb, output);
}

NDB_HybridGenerator::NDB_HybridGenerator(const DBRecord &record, int length, double probabilityRatio, double recordCountRatio)
: _probabilityRatio(probabilityRatio), _recordCountRatio(recordCountRatio)
{
    _db.push_back(record);
    _length = length;
}

NDB NDB_HybridGenerator::GenComplete(const Permutation& permutation)
{
    NDB ndb;
    auto dbRecord = permutation.Apply(_db[0].Characters());
    for (int i = _length - 1;  i >= 2; --i)
    {
        NDBRecord record{std::vector<NDBChar>(_length, NDBChar::Wildcard)};
        auto indices = _random.GetRandomIndices(i, 2);
        record.Characters()[i] = dbRecord[i] ? NDBChar::Bit0 : NDBChar::Bit1;
        record.Characters()[indices[0]] = dbRecord[indices[0]] ? NDBChar::Bit1 : NDBChar::Bit0;
        record.Characters()[indices[1]] = dbRecord[indices[1]] ? NDBChar::Bit1 : NDBChar::Bit0;
        ndb.Add(NDBRecord(permutation.Inverse(record.Characters())));
    }
    NDBRecord record{std::vector<NDBChar>(_length, NDBChar::Wildcard)};
    record.Characters()[0] = dbRecord[0] ? NDBChar::Bit1 : NDBChar::Bit0;
    record.Characters()[1] = dbRecord[1] ? NDBChar::Bit0 : NDBChar::Bit1;

    auto index = _random.GetRandomInt(2, _length - 1);
    record.Characters()[index] = NDBChar::Bit0;
    ndb.Add(NDBRecord(permutation.Inverse(record.Characters())));
    record.Characters()[index] = NDBChar::Bit1;
    ndb.Add(NDBRecord(permutation.Inverse(record.Characters())));


    index = _random.GetRandomInt(1, _length - 1);

    for (int i = 0; i < 2; ++i)
    {
        record = NDBRecord{std::vector<NDBChar>(_length, NDBChar::Wildcard)};
        record.Characters()[0] = dbRecord[0] ? NDBChar::Bit0 : NDBChar::Bit1;
        record.Characters()[index] = i == 1 ? NDBChar::Bit1 : NDBChar::Bit0;
        int j;
        while ((j = _random.GetRandomInt(1, _length-1)) == i);
        record.Characters()[j] = NDBChar::Bit0;
        ndb.Add(NDBRecord(permutation.Inverse(record.Characters())));
        record.Characters()[j] = NDBChar::Bit1;
        ndb.Add(NDBRecord(permutation.Inverse(record.Characters())));
    }

    return ndb;
}

std::size_t NDB_HybridGenerator::MakeHardReverse(const NDB &ndb, Stream &output)
{
    std::size_t count = _length + 3, n = std::ceil(_recordCountRatio * _length);
    const auto& dbRecord = _db[0];
    while (count < n)
    {
        auto indices = _random.GetRandomIndices(_length, 3);
        NDBRecord record {std::vector<NDBChar>(_length, NDBChar::Wildcard)};
        int u = 0;
        for (int index : indices)
        {
            bool randBit = _random.GetRandomInt(0, 1) == 1;
            record.Characters()[index] = randBit ? NDBChar::Bit1 : NDBChar::Bit0;
            if (randBit != dbRecord.Characters()[index]) ++u;
        }
        if (u > 0 && std::pow(_probabilityRatio, u) >= _random.GetRandomDouble(0, 1))
        {
            output << record.ToString() << "\n";
            ++count;
            std::cout << "\r" << count << "/" << n << "    ";
        }
    }
    std::cout << std::endl;

    return count;
}

std::string NDB_HybridGenerator::GetName()
{
    return "Hybrid";
}

void NDB_HybridGenerator::PrintParameters() const
{
    std::cout << "Algorithm: " << GetName() << std::endl;
    std::cout << "l: " << _length << std::endl;
    std::cout << "q: " << _probabilityRatio << std::endl;
    std::cout << "r: " << _recordCountRatio << std::endl;
}

FileUtils::CsvFileData NDB_HybridGenerator::GetCsvData() const
{
    return FileUtils::CsvFileData {.algorithm=GetName(), .length = _length, .probabilityRatio = _probabilityRatio, .recordCountRatio = _recordCountRatio };
}

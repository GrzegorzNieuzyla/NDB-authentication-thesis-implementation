#include "SuperfluousStringTest.h"
#include "../Generators/NDB_QHiddenGenerator.h"
#include "../Generators/NDB_KHiddenGenerator.h"
#include "../Streams/NDBStream.h"
#include "../Interfaces/ZChaffInterface.h"
#include <boost/uuid/detail/md5.hpp>
#include <boost/crc.hpp>
#include <utility>
#include <numeric>


using crc8 = boost::crc_optimal<8, 0xD5, 0, 0, true, true>;
using boost::uuids::detail::md5;


SuperfluousStringTest::SuperfluousStringTest(SuperfluousStringTest::ChecksumType checksumType, int length,
                                             double probabilityRatio, double recordCountRatio, int specifiedBits)
        : _checksumType(checksumType), _length(length), _probabilityRatio(probabilityRatio),
          _recordCountRatio(recordCountRatio), _specifiedBits(specifiedBits)
{
    _algorithm = Algorithm::QHidden;
}

SuperfluousStringTest::SuperfluousStringTest(SuperfluousStringTest::ChecksumType checksumType, int length,
     std::vector<double> probabilityRatios, double recordCountRatio, int specifiedBits)
: _checksumType(checksumType), _length(length), _probabilityRatios(std::move(probabilityRatios)), _recordCountRatio(recordCountRatio), _specifiedBits(specifiedBits)
{
    _algorithm = Algorithm::KHidden;
}

void SuperfluousStringTest::Run()
{
    RandomValuesGenerator rng;
    auto db = *rng.GenerateRandomDB(1, _length).begin();
    Pad(db);
    NDBStream ndb;
    std::unique_ptr<NDB_Generator> generator;
    if (_algorithm == Algorithm::QHidden)
    {
        generator = std::unique_ptr<NDB_Generator>(new NDB_QHiddenGenerator(db, db.Size(), _probabilityRatio, _recordCountRatio, _specifiedBits));
    }
    else if (_algorithm == Algorithm::KHidden)
    {
        generator = std::unique_ptr<NDB_Generator>(new NDB_KHiddenGenerator(db, db.Size(), _probabilityRatios, _recordCountRatio, _specifiedBits));
    }
    generator->Generate(ndb);
    auto record = ZChaffInterface().Solve(ndb.Ndb());
    int matched = 0, unmatched = 0;
    while (record.has_value())
    {
        if (record.value().Characters() == db.Characters())
        {
            std::cout << "Found encoded record: " << record->ToString() << std::endl;
        }
        else if (Check(record.value()))
        {
            std::cout << "Found superfluous matched record: " << record->ToString() << " : " << NormalizeHammingDistance(record.value(), db) << std::endl;
            matched++;
        }
        else
        {
            std::cout << "Found superfluous unmatched record: " << record->ToString() << " : " << NormalizeHammingDistance(record.value(), db) <<std::endl;
            unmatched++;
        }
        ExcludeRecord(ndb.Ndb(), record.value());
        record = ZChaffInterface().Solve(ndb.Ndb());
    }
    std::cout << "Found " << matched << " false positives (+ " << unmatched << " unmatched)" << std::endl;

}

bool SuperfluousStringTest::Check(const DBRecord& record)
{
    switch(_checksumType)
    {
        case ChecksumType::None:
            return true;
        case ChecksumType::ParityBit:
        {
            return std::accumulate(record.begin(), record.begin() + (record.Size() - 1), false, [](bool a, bool b){ return a != b ;}) == record.Characters()[record.Size()-1];
        }
        case ChecksumType::CRC8:
        {
            auto checksum = DBRecord(std::vector<bool>(record.end() - 8, record.end())).ToBytes();
            auto rawRecord = DBRecord(std::vector<bool>(record.begin(), record.end() - 8)).ToBytes();
            crc8 crc;
            crc.process_bytes(rawRecord.data(), rawRecord.size());
            auto crcChecksum = crc.checksum();
            return *reinterpret_cast<crc8::value_type*>(checksum.data()) == crcChecksum;
        }
        case ChecksumType::CRC16:
        {
            auto checksum = DBRecord(std::vector<bool>(record.end() - 16, record.end())).ToBytes();
            auto rawRecord = DBRecord(std::vector<bool>(record.begin(), record.end() - 16)).ToBytes();
            boost::crc_16_type crc;
            crc.process_bytes(rawRecord.data(), rawRecord.size());
            auto crcChecksum = crc.checksum();
            return *reinterpret_cast<boost::crc_16_type::value_type*>(checksum.data()) == crcChecksum;
        }
        case ChecksumType::CRC32:
        {
            auto checksum = DBRecord(std::vector<bool>(record.end() - 32, record.end())).ToBytes();
            auto rawRecord = DBRecord(std::vector<bool>(record.begin(), record.end() - 32)).ToBytes();
            boost::crc_32_type crc;
            crc.process_bytes(rawRecord.data(), rawRecord.size());
            auto crcChecksum = crc.checksum();
            return *reinterpret_cast<boost::crc_32_type::value_type*>(checksum.data()) == crcChecksum;
        }
        case ChecksumType::MD5:
        {
            auto checksum = DBRecord(std::vector<bool>(record.end() - 128, record.end())).ToBytes();
            auto rawRecord = DBRecord(std::vector<bool>(record.begin(), record.end() - 128)).ToBytes();
            md5 hash;
            md5::digest_type digest;
            hash.process_bytes(rawRecord.data(), rawRecord.size());
            hash.get_digest(digest);
            std::vector<unsigned char> digestBytes(reinterpret_cast<unsigned char*>(digest), reinterpret_cast<unsigned char*>(&digest) + sizeof(digest));
            return digestBytes == checksum;
        }
    }
    return false;
}

void SuperfluousStringTest::Pad(DBRecord &record)
{
    assert(record.Size() % 8 == 0);
    switch (_checksumType)
    {
        case ChecksumType::None:
            return;
        case ChecksumType::ParityBit:
            record.Characters().push_back(std::accumulate(record.begin(), record.end(), false, [](bool a, bool b){ return a != b ;}));
            return;
        case ChecksumType::CRC8:
        {
            crc8 crc;
            auto bytes = record.ToBytes();
            crc.process_bytes(bytes.data(), bytes.size());
            auto checksum = crc.checksum();
            std::vector<unsigned char> crcBytes(reinterpret_cast<unsigned char*>(&checksum), reinterpret_cast<unsigned char*>(&checksum) + sizeof(checksum));
            record.AppendBytes(crcBytes);
            return;
        }
        case ChecksumType::CRC16:
        {
            boost::crc_16_type crc;
            auto bytes = record.ToBytes();
            crc.process_bytes(bytes.data(), bytes.size());
            auto checksum = crc.checksum();
            std::vector<unsigned char> crcBytes(reinterpret_cast<unsigned char*>(&checksum), reinterpret_cast<unsigned char*>(&checksum) + sizeof(checksum));
            record.AppendBytes(crcBytes);
            return;
        }
        case ChecksumType::CRC32:
        {
            boost::crc_32_type crc;
            auto bytes = record.ToBytes();
            crc.process_bytes(bytes.data(), bytes.size());
            auto checksum = crc.checksum();
            std::vector<unsigned char> crcBytes(reinterpret_cast<unsigned char*>(&checksum), reinterpret_cast<unsigned char*>(&checksum) + sizeof(checksum));
            record.AppendBytes(crcBytes);
            return;
        }
        case ChecksumType::MD5:
        {
            md5 hash;
            md5::digest_type digest;
            auto bytes = record.ToBytes();
            hash.process_bytes(bytes.data(), bytes.size());
            hash.get_digest(digest);
            std::vector<unsigned char> md5bytes(reinterpret_cast<unsigned char*>(digest), reinterpret_cast<unsigned char*>(digest) + 16);
            record.AppendBytes(md5bytes);
            return;
        }
    }
}

void SuperfluousStringTest::ExcludeRecord(NDB &ndb, const DBRecord &record)
{
    NDBRecord ndbRecord;
    for (auto ch : record)
        ndbRecord.Characters().push_back(ch ? NDBChar::Bit1 : NDBChar::Bit0);
    ndb.Add(ndbRecord);
}

SuperfluousStringTest::ChecksumType SuperfluousStringTest::GetChecksumType(int bits)
{
    switch (bits)
    {
        case 0: return ChecksumType::None;
        case 1: return ChecksumType::ParityBit;
        case 8: return ChecksumType::CRC8;
        case 16: return ChecksumType::CRC16;
        case 32: return ChecksumType::CRC32;
        case 128: return ChecksumType::MD5;
        default: throw std::invalid_argument("Invalid checksum length");
    }
}

double SuperfluousStringTest::NormalizeHammingDistance(const DBRecord &r1, const DBRecord &r2)
{
    assert(r1.Size() == r2.Size());
    double diffs = 0;
    for (int i = 0; i < r1.Size(); ++i)
    {
        if (r1.Characters()[i] != r2.Characters()[i])
            ++diffs;
    }
    return diffs / r1.Size();
}

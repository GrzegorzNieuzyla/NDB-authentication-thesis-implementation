#pragma once
#include <boost/uuid/detail/md5.hpp>
#include <boost/crc.hpp>
#include <numeric>
#include "../DB.h"
using crc8 = boost::crc_optimal<8, 0xD5, 0, 0, true, true>;

class Checksum
{
public:
    enum class ChecksumType
    {
        None,
        ParityBit,
        CRC8,
        CRC16,
        CRC32,
        MD5
    };
    static bool Check(const DBRecord& record, ChecksumType checksumType)
    {
        using boost::uuids::detail::md5;

        switch(checksumType)
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

    static void Pad(DBRecord &record, ChecksumType checksumType)
    {
        using boost::uuids::detail::md5;

        assert(record.Size() % 8 == 0);
        switch (checksumType)
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
    static ChecksumType GetChecksumType(int bits)
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
    static int GetChecksumBits(ChecksumType type)
    {
        switch (type)
        {
            case ChecksumType::None:
                return 0;
            case ChecksumType::ParityBit:
                return 1;
            case ChecksumType::CRC8:
                return 8;
            case ChecksumType::CRC16:
                return 16;
            case ChecksumType::CRC32:
                return 32;
            case ChecksumType::MD5:
                return 128;
        }
    }
};
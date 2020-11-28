#pragma once


#include <vector>
#include "../DB.h"
#include "../NDB.h"

class SuperfluousStringTest
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

    SuperfluousStringTest(ChecksumType checksumType, int length, double probabilityRatio, double recordCountRatio,
                          int specifiedBits);

    SuperfluousStringTest(ChecksumType checksumType, int length, std::vector<double> probabilityRatios,
                          double recordCountRatio, int specifiedBits);

    void Run();
    static ChecksumType GetChecksumType(int bits);
private:
    enum class Algorithm
    {
        QHidden,
        KHidden
    };
    bool Check(const DBRecord& record);
    void Pad(DBRecord& record);
    static void ExcludeRecord(NDB& ndb, const DBRecord& record);
    static double NormalizeHammingDistance(const DBRecord& r1, const DBRecord& r2);
    Algorithm _algorithm;
    ChecksumType _checksumType;
    int _length;
    double _probabilityRatio;
    std::vector<double> _probabilityRatios;
    double _recordCountRatio;
    int _specifiedBits;
};




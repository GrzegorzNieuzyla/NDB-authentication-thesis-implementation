#pragma once


#include <vector>
#include "../DB.h"
#include "../NDB.h"
#include "../Utils/Checksum.h"
#include "../Utils/FileUtils.h"


class SuperfluousStringTest
{
public:
    SuperfluousStringTest(Checksum::ChecksumType checksumType, int length, double probabilityRatio, double recordCountRatio,
                          int specifiedBits);

    SuperfluousStringTest(Checksum::ChecksumType checksumType, int length, std::vector<double> probabilityRatios,
                          double recordCountRatio, int specifiedBits);


    struct Results
    {
        int unmatched = 0;
        int matched = 0;
        double avgHammingDistance = 0;
        FileUtils::CsvFileData csvData{};
    };

    Results Run();
private:
    enum class Algorithm
    {
        QHidden,
        KHidden,
    };
    static void ExcludeRecord(NDB& ndb, const DBRecord& record);
    static double NormalizeHammingDistance(const DBRecord& r1, const DBRecord& r2);
    Algorithm _algorithm;
    Checksum::ChecksumType _checksumType;
    int _length;
    double _probabilityRatio;
    std::vector<double> _probabilityRatios;
    double _recordCountRatio;
    int _specifiedBits;
};




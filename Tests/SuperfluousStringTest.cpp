#include "SuperfluousStringTest.h"
#include "../Generators/NDB_QHiddenGenerator.h"
#include "../Generators/NDB_KHiddenGenerator.h"
#include "../Streams/NDBStream.h"
#include "../Interfaces/ZChaffInterface.h"
#include <boost/uuid/detail/md5.hpp>
#include <boost/crc.hpp>
#include <utility>
#include <numeric>


using ChecksumType = Checksum::ChecksumType;

SuperfluousStringTest::SuperfluousStringTest(ChecksumType checksumType, int length,
                                             double probabilityRatio, double recordCountRatio, int specifiedBits)
        : _checksumType(checksumType), _length(length), _probabilityRatio(probabilityRatio),
          _recordCountRatio(recordCountRatio), _specifiedBits(specifiedBits)
{
    _algorithm = Algorithm::QHidden;
}

SuperfluousStringTest::SuperfluousStringTest(ChecksumType checksumType, int length,
     std::vector<double> probabilityRatios, double recordCountRatio, int specifiedBits)
: _checksumType(checksumType), _length(length), _probabilityRatios(std::move(probabilityRatios)), _recordCountRatio(recordCountRatio), _specifiedBits(specifiedBits)
{
    _algorithm = Algorithm::KHidden;
}

SuperfluousStringTest::Results SuperfluousStringTest::Run()
{
    auto db = *RandomValuesGenerator::GenerateRandomDB(1, _length).begin();
    Checksum::Pad(db, _checksumType);
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
    auto solution = ZChaffInterface().Solve(ndb.Ndb());
    Results results;
    while (solution.record.has_value())
    {
        if (solution.record.value().Characters() == db.Characters())
        {
            std::cout << "Found encoded record: " << solution.record->ToString() << std::endl;
        }
        else if (Checksum::Check(solution.record.value(), _checksumType))
        {
            auto dist = NormalizeHammingDistance(solution.record.value(), db);
            std::cout << "Found superfluous matched record: " << solution.record->ToString() << " "<< results.matched <<  " : " << dist << std::endl;
            results.matched++;
            results.avgHammingDistance += dist;
        }
        else
        {
            auto dist = NormalizeHammingDistance(solution.record.value(), db);
            std::cout << "Found superfluous unmatched record: " << solution.record->ToString() << " : " << dist <<std::endl;
            results.unmatched++;
            results.avgHammingDistance += dist;
        }
        ExcludeRecord(ndb.Ndb(), solution.record.value());
        solution = ZChaffInterface().Solve(ndb.Ndb());
    }
    std::cout << "Found " << results.matched << " false positives (+ " << results.unmatched << " unmatched)" << std::endl;
    if (results.avgHammingDistance > 0)
        results.avgHammingDistance /= (results.unmatched + results.matched);
    results.csvData = generator->GetCsvData();
    return results;
}

void SuperfluousStringTest::ExcludeRecord(NDB &ndb, const DBRecord &record)
{
    NDBRecord ndbRecord;
    for (auto ch : record)
        ndbRecord.Characters().push_back(ch ? NDBChar::Bit1 : NDBChar::Bit0);
    ndb.Add(ndbRecord);
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

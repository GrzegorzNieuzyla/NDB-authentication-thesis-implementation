#include <iostream>
#include "Generators/NDBPrefixGenerator.h"
#include "Generators/NDBRandomizedGenerator.h"
#include "Generators/NDBRandomizedGenerator2.h"
#include "Validator/NDBValidator.h"
#include "Utils/Benchmark.h"
#include "Streams/FileStream.h"
#include "Streams/NDBStream.h"
#include "Streams/DimacsFileStream.h"
#include <cstdlib>
#include <chrono>
#include <algorithm>
#include <fstream>

const DB ReferenceDB = {{0,0,0,1}, {0,1,0,0}, {1,0,0,0}, {1,0,1,1}};
const DB ReferenceDB2 = {{0,0,0,0}, {0,1,1,0}, {0,0,1,0}, {1,1,0,1}};

void Assert(bool value)
{
    if (!value) throw;
}

void PrintNDB(const NDB& ndb)
{
    for (const auto& record : ndb.Records())
    {
        std::cout << record.ToString() << std::endl;
    }
}

void PrintDB(const DB& db)
{
    for (const auto& record : db)
    {
        for (const auto ch : record)
        {
            std::cout << ch;
        }
        std::cout << std::endl;
    }
}

std::set<DBRecord> GenerateRandomDB(int count, int length)
{
    std::set<DBRecord> db;
    for (int i = 0; i < count; i++)
    {
        DBRecord record;
        for (int j = 0; j < length; j++)
        {
            record.Characters().emplace_back(rand() % 2 == 1);
        }
        db.emplace(std::move(record));
    }
    return db;
}

template <class Generator>
std::tuple<size_t, size_t, std::string> DoBenchmark(int count, int length)
{
    auto db = GenerateRandomDB(count, length);
    auto filename = "../_tests/SAT/ndb_" + Generator::GetName() + "_" + std::to_string(count) + "_" + std::to_string(length)
            + ".dimacs";
    DimacsFileStream file(filename);

    Benchmark benchmark;
    benchmark.Start();
    auto record_count = Generator(db, length).Generate(file);
    auto elapsed_time = benchmark.GetElapsedTime();
    file.WriteHeader(length, record_count);
    auto memory = Benchmark::GetMemoryUsage();
//    Assert(NDBValidator::ValidateFromFile(db, filename));
    return {elapsed_time, record_count, memory};
}

int main()
{
//    auto db = GenerateRandomDB(1, 20);
//    NDBStream ndb;
//    NDBRandomizedGenerator2(db, db.begin()->Characters().size()).Generate(ndb);
//    NDBValidator(ndb.Ndb(), db).ValidateAllDBRecords();
    std::vector<std::pair<int, int>> cases =
            {
                    {1,  2048},
                    {500,  200},
                    {500,  500},
                    {500,  1000},
                    {1000, 100},
                    {2000, 100},
                    {2000, 200},
                    {1000, 512},
                    {2000, 512},
                    {10000, 512},
                    {10, 10000}
            };

    for (const auto& _case : cases)
    {
        auto result = DoBenchmark<NDBRandomizedGenerator2>(_case.first, _case.second);
        std::cout << _case.first << ", " << _case.second << " - " << std::get<0>(result) << ", " << std::get<1>(result) << ", " << std::get<2>(result) << std::endl;
        break;
    }
}

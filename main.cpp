#include <iostream>
#include "Generators/NDB_PrefixGenerator.h"
#include "Generators/NDB_RandomizedGenerator.h"
#include "Generators/NDB_RandomizedGenerator2.h"
#include "Validator/NDBValidator.h"
#include "Utils/Benchmark.h"
#include "Streams/FileStream.h"
#include "Streams/NDBStream.h"
#include "Streams/DimacsFileStream.h"
#include "Generators/NDB_QHiddenGenerator.h"
#include "Generators/NDB_KHiddenGenerator.h"
#include "Generators/NDB_HybridGenerator.h"
#include <cstdlib>
#include <chrono>
#include <algorithm>
#include <fstream>

const DB ReferenceDB = {{0,0,0,1}, {0,1,0,0}, {1,0,0,0}, {1,0,1,1}};
const DB ReferenceDB2 = {{0,0,0,0}, {0,1,1,0}, {0,0,1,0}, {1,1,0,1}};

const DBRecord ReferenceRecord = {1,0,0,1,0};

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
    RandomValuesGenerator _rng;
    std::set<DBRecord> db;
    for (int i = 0; i < count; i++)
    {
        DBRecord record;
        for (int j = 0; j < length; j++)
        {
            record.Characters().emplace_back(_rng.GetRandomInt(0, 1) == 1);
        }
        db.emplace(std::move(record));
    }
    return db;
}

template <class Generator>
std::tuple<size_t, size_t, std::string> DoBenchmark(int count, int length)
{
    auto db = GenerateRandomDB(1, length);
    auto filename = "../_tests/SAT/ndb_" + Generator::GetName() + "_" + std::to_string(count) + "_" + std::to_string(length)
            + ".dimacs";
    DimacsFileStream file(filename);

    Benchmark benchmark;
    benchmark.Start();
    //std::vector<double> prop = { 0.5, 0.9 };
    auto record_count = Generator(*db.begin(), length).Generate(file);
    auto elapsed_time = benchmark.GetElapsedTime();
    file.WriteHeader(length, record_count);
    auto memory = Benchmark::GetMemoryUsage();
    PrintDB(db);
    //Assert(NDBValidator::ValidateFromFile(db, filename));
    return {elapsed_time, record_count, memory};
}

int main()
{
    std::vector<std::pair<int, int>> cases =
            {
                    {20,  300},
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
        auto result = DoBenchmark<NDB_QHiddenGenerator>(_case.first, _case.second);
        std::cout << _case.first << ", " << _case.second << " - " << std::get<0>(result) << ", " << std::get<1>(result) << ", " << std::get<2>(result) << std::endl;
        break;
    }
}

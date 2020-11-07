#include <iostream>
#include "Generators/NDBPrefixGenerator.h"
#include "Generators/NDBRandomizedGenerator.h"
#include "Validator/NDBValidator.h"
#include "Utils/Benchmark.h"
#include <cstdlib>
#include <chrono>
#include <algorithm>
#include <fstream>

const std::set<DBRecord> ReferenceDB = {{0,0,0,1}, {0,1,0,0}, {1,0,0,0}, {1,0,1,1}};
const std::set<DBRecord> ReferenceDB2 = {{0,0,0,0}, {0,1,1,0}, {0,0,1,0}, {1,1,0,1}};

void Assert(bool value)
{
    if (!value) throw;
}

void PrintNDB(NDB ndb)
{
    for (const auto& record : ndb.Records())
    {
        std::cout << record.ToString() << std::endl;
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
std::tuple<size_t, size_t, std::string> DoBenchmark(int count, int length, bool output = false)
{
    auto db = GenerateRandomDB(count, length);
    auto filename = "../_tests/ndb_" + Generator::GetName() + "_" + std::to_string(count) + "_" + std::to_string(length);
    std::ofstream file(filename);

    Benchmark benchmark;
    benchmark.Start();
    auto record_count = Generator(db, length).GenerateToFile(file);
    auto elapsed_time = benchmark.GetElapsedTime();

    auto memory = Benchmark::GetMemoryUsage();
    Assert(NDBValidator::ValidateFromFile(db, filename));
    return {elapsed_time, record_count, memory};
}

int main()
{
    auto ndb = NDBRandomizedGenerator(ReferenceDB, ReferenceDB.begin()->Characters().size()).Generate();
    PrintNDB(ndb);
    std::vector<std::pair<int, int>> cases =
            {
                    {50,  128},
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
        auto result = DoBenchmark<NDBRandomizedGenerator>(_case.first, _case.second, true);
        std::cout << _case.first << ", " << _case.second << " - " << std::get<0>(result) << ", " << std::get<1>(result) << ", " << std::get<2>(result) << std::endl;
    }
}

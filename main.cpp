#include <iostream>
#include "Generators/NDBPrefixGenerator.h"
#include "Validator/NDBValidator.h"
#include <cstdlib>
#include <chrono>
#include <algorithm>
#include <cassert>

const std::set<DBRecord> ReferenceDB = {{0,0,0,1}, {0,1,0,0}, {1,0,0,0}, {1,0,1,1}};
const std::set<DBRecord> ReferenceDB2 = {{0,0,0,0}, {0,1,1,0}, {0,0,1,0}, {1,1,0,1}};

void PrintNDB(NDB ndb)
{
    for (auto record : ndb.Records())
    {
        for (auto ch : record.Characters())
        {
            std::cout << (ch == NDBChar::Bit0 ? '0' : (ch == NDBChar::Bit1 ? '1' : '*'));
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
std::pair<size_t, size_t> Benchmark(int count, int length)
{
    auto db = GenerateRandomDB(count, length);
    auto t_start = std::chrono::high_resolution_clock::now();
    auto ndb = Generator(db, length).Generate();
    auto t_end = std::chrono::high_resolution_clock::now();
    auto elapsed_time = std::chrono::duration<double>(t_end - t_start).count();
    assert(NDBValidator(ndb, db).ValidateAllDBRecords());
    return {elapsed_time, ndb.Records().size()};
}

int main()
{
    auto ndb = NDBPrefixGenerator(ReferenceDB2, ReferenceDB2.begin()->Characters().size()).Generate();
    PrintNDB(ndb);
    std::vector<std::pair<int, int>> cases =
            {
                    {100,  512},
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
        auto result = Benchmark<NDBPrefixGenerator>(_case.first, _case.second);
        std::cout << _case.first << ", " << _case.second << " - " << result.first << ", " << result.second << std::endl;
    }
}

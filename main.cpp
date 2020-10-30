#include <iostream>
#include "Generators/NDBPrefixGenerator.h"
#include <cstdlib>
#include <chrono>

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
    for (int i = 0; i < count; i++){
        DBRecord record;
        for (int j = 0; j < length; j++)
        {
            record.emplace_back(rand() % 2 == 1);
        }
        db.emplace(std::move(record));
    }
    return db;
}

int main()
{
    std::set<DBRecord> db = {{0,0,0,1}, {0,1,0,0}, {1,0,0,0}, {1,0,1,1}};
    //db = GenerateRandomDB(3000, 512);

    auto t_start = std::chrono::high_resolution_clock::now();
    auto ndb = NDBPrefixGenerator(db, db.begin()->size()).Generate();
    auto t_end = std::chrono::high_resolution_clock::now();

    double elapsed_time = std::chrono::duration<double>(t_end-t_start).count();
    std::cout << "Time elapsed: " << elapsed_time << std::endl;
    std::cout << ndb.Records().size() << std::endl;

    PrintNDB(ndb);
}

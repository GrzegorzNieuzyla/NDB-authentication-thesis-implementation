#include <iostream>
#include "NDBGenerator.h"
#include <cstdlib>
#include <chrono>

void printNDB(NDB ndb)
{
    for (auto record : ndb.records())
    {
        for (auto ch : record.characters())
        {
            std::cout << (ch == NDBChar::Bit0 ? '0' : (ch == NDBChar::Bit1 ? '1' : '*'));
        }
        std::cout << std::endl;
    }
}

int main()
{
    std::set<DBRecord> db = {{0,0,0,1}, {0,1,0,0}, {1,0,0,0}, {1,0,1,1}};
    for (int i = 0; i < 500; i++){
        DBRecord record;
        for (int j = 0; j < 100; j++)
        {
            record.emplace_back(rand() % 2 == 1);
        }
        db.emplace(std::move(record));
    }

    auto t_start = std::chrono::high_resolution_clock::now();
    auto ndb = NDBPrefixGenerator(db).generate(db.begin()->size());
    auto t_end = std::chrono::high_resolution_clock::now();

    double elapsed_time = std::chrono::duration<double>(t_end-t_start).count();
    std::cout << "Time elapsed: " << elapsed_time << std::endl;
    std::cout << ndb.records().size() << std::endl;

   // printNDB(ndb);
}

#pragma once


#include <string>

class GenerationTimeTest
{
public:
    GenerationTimeTest() = default;
    void RunTests(const std::string& filename);

private:
    static void TestQHidden(std::ofstream& file);

    static void TestKHidden(std::ofstream &file);

    static void TestHybrid(std::ofstream &file);
};




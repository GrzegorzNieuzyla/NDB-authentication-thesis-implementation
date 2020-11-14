#pragma once
#include <ostream>
#include <fstream>
#include "Stream.h"
#include <cassert>

class DimacsFileStream : public Stream
{
public:
    explicit DimacsFileStream(const std::string& filename);
    Stream& operator<<(const std::string &string) override;
    void WriteHeader(int length, int count);
private:
    const int HEADER_SIZE = 64;
    const std::string HEADER_PART = "p cnf ";
    std::ofstream _file;
};
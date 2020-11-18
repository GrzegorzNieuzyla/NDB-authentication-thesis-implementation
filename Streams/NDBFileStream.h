#pragma once
#include <fstream>
#include "Stream.h"

class NDBFileStream : public Stream
{
public:
    explicit NDBFileStream(const std::string& filename);
    Stream &operator<<(const std::string &string) override;

private:
    std::ofstream _file;
};



#pragma once
#include <fstream>
#include "Stream.h"

class FileStream : public Stream
{
public:
    explicit FileStream(const std::string& filename);
    Stream &operator<<(const std::string &string) override;

private:
    std::ofstream _file;
};



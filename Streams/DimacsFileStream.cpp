#include <iostream>
#include "DimacsFileStream.h"

DimacsFileStream::DimacsFileStream(const std::string &filename)
{
    _file.open(filename);
    assert(_file.is_open());
    _file << HEADER_PART << std::string(HEADER_SIZE, '-') << '\n';
}

Stream &DimacsFileStream::operator<<(const std::string &string)
{
    if (string == "\n")
    {
        _file << string;
        return *this;
    }
    int var = 1;
    for (auto ch : string)
    {
        if (ch == '0')
            _file << std::to_string(var) << ' ';
        if (ch == '1')
            _file << '-' << std::to_string(var) << ' ';
        ++var;
    }
    _file << "0";
    return *this;
}

void DimacsFileStream::WriteHeader(int length, int count)
{
    _file.seekp(HEADER_PART.size());
    std::string header = std::to_string(length) + " " + std::to_string(count);
    header += std::string(HEADER_SIZE - header.size(), ' ') + '\n';
    _file << header;
    _file.flush();
    _headerWritten = true;
}

DimacsFileStream::~DimacsFileStream()
{
    if (!_headerWritten)
    {
        std::cout << "DimacsFileStream::WriteHeader() not called" << std::endl;
        assert(false);
    }
}

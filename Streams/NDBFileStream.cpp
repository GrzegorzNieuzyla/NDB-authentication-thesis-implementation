#include "NDBFileStream.h"

Stream &NDBFileStream::operator<<(const std::string &string)
{
    _file << string;
    return *this;
}

NDBFileStream::NDBFileStream(const std::string &filename) : _file(filename) {}

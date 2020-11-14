#include "FileStream.h"

Stream &FileStream::operator<<(const std::string &string)
{
    _file << string;
    return *this;
}

FileStream::FileStream(const std::string &filename) : _file(filename) {}

#include "DummyStream.h"

Stream &DummyStream::operator<<(const std::string &string)
{
    return *this;
}

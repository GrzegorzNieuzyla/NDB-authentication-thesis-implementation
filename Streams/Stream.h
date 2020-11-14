#pragma once
#include <string>

class Stream
{
public:
    virtual Stream& operator<<(const std::string& string) = 0;
};
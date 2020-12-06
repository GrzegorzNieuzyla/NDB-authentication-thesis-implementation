#pragma once


#include "Stream.h"

class DummyStream : public Stream
{
public:
    Stream &operator<<(const std::string &string) override;

};




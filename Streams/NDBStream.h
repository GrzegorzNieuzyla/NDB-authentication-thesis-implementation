#pragma once

#include <unordered_set>
#include "Stream.h"
#include "../NDB.h"

class NDBStream : public Stream
{
public:
    Stream &operator<<(const std::string &string) override;
    [[nodiscard]] NDB& Ndb();
private:
    NDB _ndb;
};
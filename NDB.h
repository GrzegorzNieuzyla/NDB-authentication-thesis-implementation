#pragma once
#include <vector>

enum class NDBChar
{
    Bit0 = 1,
    Bit1 = 2,
    Wildcard = 3
};


class NDBRecord
{
public:
    NDBRecord() = default;
    explicit NDBRecord(std::vector<NDBChar> characters);
    NDBChar& operator[](int index);
    [[nodiscard]] std::size_t Size() const;
    std::vector<NDBChar>& Characters();
private:
    std::vector<NDBChar> _characters;
};

class NDB
{
public:
    void add(NDBRecord record);
    std::vector<NDBRecord>& Records();
private:
    std::vector<NDBRecord> _records;
};
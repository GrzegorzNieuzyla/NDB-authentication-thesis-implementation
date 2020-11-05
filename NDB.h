#pragma once
#include <vector>
#include <string>

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
    [[nodiscard]] const std::vector<NDBChar>& Characters() const;

    [[nodiscard]] std::string ToString() const;

private:
    std::vector<NDBChar> _characters;
};

class NDB
{
public:
    void add(NDBRecord record);
    std::vector<NDBRecord>& Records();
    [[nodiscard]] const std::vector<NDBRecord>& Records() const;
private:
    std::vector<NDBRecord> _records;
};
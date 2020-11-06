#pragma once
#include <vector>
#include <string>
#include <unordered_set>
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
    bool operator==(const NDBRecord& other) const;
private:
    std::vector<NDBChar> _characters;
};

struct NDBRecordHash
{
    std::size_t operator()(const NDBRecord& record) const
    {
        std::size_t seed = record.Size();
        for(auto& ch : record.Characters()) {
            auto i = static_cast<std::size_t>(ch);
            seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};
using NDBRecordUnorderedSet = std::unordered_set<NDBRecord, NDBRecordHash>;
class NDB
{
public:
    void add(NDBRecord record);
    std::vector<NDBRecord>& Records();
    [[nodiscard]] const std::vector<NDBRecord>& Records() const;
    [[nodiscard]] std::size_t Size() const;
    void DumpToFile(const std::string& filename) const;
private:
    std::vector<NDBRecord> _records;
};


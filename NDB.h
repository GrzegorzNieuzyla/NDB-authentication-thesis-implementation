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
    [[nodiscard]] std::size_t size() const;
    std::vector<NDBChar>& characters();
private:
    std::vector<NDBChar> _characters;
};

class NDB
{
public:
    void add(NDBRecord record);
    std::vector<NDBRecord>& records();
private:
    std::vector<NDBRecord> _records;
};
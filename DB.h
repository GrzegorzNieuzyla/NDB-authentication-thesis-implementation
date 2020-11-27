#pragma once
#include "VectorView.h"
#include <vector>
#include <string>
#include <set>
using DBRecordView = VectorView<bool>;


class DBRecord
{
public:
    DBRecord() = default;
    DBRecord(std::initializer_list<bool> list);
    explicit DBRecord(std::vector<bool> record);

    DBRecord(std::vector<bool>::iterator begin, std::vector<bool>::iterator end);
    DBRecord(std::vector<bool>::const_iterator begin, std::vector<bool>::const_iterator end);

    [[nodiscard]] const std::vector<bool>& Characters() const;
    std::vector<bool>& Characters();
    std::vector<bool>::iterator begin() noexcept;
    [[nodiscard]] std::vector<bool>::const_iterator begin() const noexcept;
    std::vector<bool>::iterator end() noexcept;
    [[nodiscard]] std::vector<bool>::const_iterator end() const noexcept;
    bool operator<(const DBRecord& other) const;
    [[nodiscard]] std::string ToString() const;
    [[nodiscard]] std::vector<unsigned char> ToBytes() const;
    [[nodiscard]] std::size_t Size() const;
    void AppendBytes(const std::vector<unsigned char>& bytes);
private:
    std::vector<bool> _record;
};

using DB = std::set<DBRecord>;

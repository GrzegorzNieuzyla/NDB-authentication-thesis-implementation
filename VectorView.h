#pragma once
#include <vector>

template <class T>
class VectorView
{
public:
    VectorView(typename std::vector<T>::const_iterator begin, typename std::vector<T>::const_iterator end) : _begin(begin), _end(end)
    {}
    typename std::vector<T>::const_iterator _begin;
    typename std::vector<T>::const_iterator _end;

    [[nodiscard]] std::size_t Size() const
    {
        return std::distance(_begin, _end);
    }
    typename std::vector<T>::const_iterator begin() const
    {
        return _begin;
    }
    typename std::vector<T>::const_iterator end() const
    {
        return _end;
    }
};

template <class T>
bool operator<(const VectorView<T> &v1, const std::vector<T> &v2)
{
    auto it1 = v1._begin;
    auto it2 = v2.begin();
    while (it1 != v1._end && it2 != v2.end())
    {
        if (*it1 != *it2)
            return *it1 < *it2;
        it1++;
        it2++;
    }
    return it1 == v1._end;
}


template <class T>
bool operator>(const VectorView<T> &v1, const std::vector<T> &v2)
{
    auto it1 = v1._begin;
    auto it2 = v2.begin();
    while (it1 != v1._end && it2 != v2.end())
    {
        if (*it1 != *it2)
            return *it1 > *it2;
        it1++;
        it2++;
    }
    return it1 != v1._end;
}


template <class T>
bool operator==(const VectorView<T> &v1, const std::vector<T> &v2)
{
    auto it1 = v1._begin;
    auto it2 = v2.begin();
    while (it1 != v1._end && it2 != v2.end())
    {
        if (*it1 != *it2)
            return false;
        it1++;
        it2++;
    }
    return it1 == v1._end && it2 == v2.end();
}

template <class T>
bool operator<(const std::vector<T> &v1, const VectorView<T> &v2)
{
    return v2 > v1;
}

template <class T>
bool operator>(const std::vector<T> &v1, const VectorView<T> &v2)
{
    return v2 < v1;
}

template <class T>
bool operator==(const std::vector<T> &v1, const VectorView<T> &v2)
{
    return v2 == v1;
}

template <class T>
bool operator!=(const std::vector<T> &v1, const VectorView<T> &v2)
{
    return !(v2 == v1);
}

template <class T>
bool operator!=(const VectorView<T> &v1, const std::vector<T> &v2)
{
    return !(v1 == v2);
}

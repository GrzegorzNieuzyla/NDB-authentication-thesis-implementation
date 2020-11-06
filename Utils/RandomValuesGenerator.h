#pragma once
#include <random>

#include "Permutation.h"
#include <vector>
#include <chrono>


class RandomValuesGenerator
{
public:
    RandomValuesGenerator() : RandomValuesGenerator(std::chrono::system_clock::now().time_since_epoch().count()) {}
    explicit RandomValuesGenerator(std::size_t seed)
    : _rng(seed)
    {}
    Permutation GenerateRandomPermutation(int size)
    {
        std::vector<int> permutations;
        permutations.resize(size);
        for (int i = 0; i < size; ++i)
        {
            permutations[i] = i;
        }
        std::shuffle(permutations.begin(), permutations.end(), _rng);

        return Permutation(permutations);
    }
    int GetRandomInt(int lower, int upper)
    {
        return lower + _rng() % (upper - lower + 1);
    }

    template<class T> std::vector<T> GetRandomChoice(const std::vector<T>& container, int count)
    {
        if (count >= container.size()) return container;
        if (count >= container.size() / 4)
        {
            auto cont = container;
            std::shuffle(cont.begin(), cont.end(), _rng);
            return {cont.begin(), cont.begin() + count};
        }
        std::vector<T> result;
        std::unordered_set<int> indices;
        while (result.size() != count)
        {
            auto index = GetRandomInt(0, container.size() - 1);
            if (indices.find(index) == indices.end())
            {
                result.push_back(container[index]);
            }
        }
        return result;
    }
private:
    std::mt19937 _rng;
};




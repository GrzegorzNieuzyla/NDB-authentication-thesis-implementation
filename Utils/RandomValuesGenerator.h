#pragma once
#include <random>

#include "Permutation.h"
#include "RandomSeedProvider.h"
#include <vector>
#include <chrono>


class RandomValuesGenerator
{
public:
    RandomValuesGenerator() : _rng(RandomSeedProvider::Get()) {}
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
        std::uniform_int_distribution<int> dist(lower, upper);
        return dist(_rng);
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
                indices.emplace(index);
            }
        }
        return result;
    }

    std::vector<int> GetRandomIndices(int length, int count)
    {
        std::vector<int> indices;
        indices.resize(length);
        for (int i = 0; i < length; ++i)
        {
            indices[i] = i;
        }
        return GetRandomChoice(indices, count);
    }

    double GetRandomDouble(double lower, double upper)
    {
        std::uniform_real_distribution<double> dist(lower, upper);
        return dist(_rng);
    }

private:
    std::mt19937 _rng;
};




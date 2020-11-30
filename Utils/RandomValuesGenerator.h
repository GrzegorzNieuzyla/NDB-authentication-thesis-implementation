#pragma once
#include <random>

#include "Permutation.h"
#include "RandomSeedProvider.h"
#include "../DB.h"
#include <vector>
#include <chrono>
#include <unordered_set>
#include <set>

class RandomValuesGenerator
{
public:
    static Permutation GenerateRandomPermutation(int size)
    {
        std::vector<int> permutations;
        permutations.resize(size);
        for (int i = 0; i < size; ++i)
        {
            permutations[i] = i;
        }
        std::shuffle(permutations.begin(), permutations.end(), _instance._rng);

        return Permutation(permutations);
    }
    static int GetRandomInt(int lower, int upper)
    {
        std::uniform_int_distribution<int> dist(lower, upper);
        return dist(_instance._rng);
    }

    template<class T> static  std::vector<T> GetRandomChoice(const std::vector<T>& container, int count)
    {
        assert(count <= container.size());
        if (count == container.size()) return container;
        if (count >= container.size() / 4)
        {
            auto cont = container;
            std::shuffle(cont.begin(), cont.end(), _instance._rng);
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

    static std::vector<int> GetRandomIndices(int length, int count)
    {
        std::vector<int> indices;
        indices.resize(length);
        for (int i = 0; i < length; ++i)
        {
            indices[i] = i;
        }
        return GetRandomChoice(indices, count);
    }

    static double GetRandomDouble(double lower, double upper)
    {
        std::uniform_real_distribution<double> dist(lower, upper);
        return dist(_instance._rng);
    }

    static std::set<DBRecord> GenerateRandomDB(int count, int length)
    {
        std::set<DBRecord> db;
        for (int i = 0; i < count; i++)
        {
            DBRecord record;
            for (int j = 0; j < length; j++)
            {
                record.Characters().emplace_back(GetRandomInt(0, 1) == 1);
            }
            db.emplace(std::move(record));
        }
        return db;
    }

    static size_t GetRandomULong()
    {
        std::uniform_int_distribution<std::size_t> dist;
        return dist(_instance._rng);
    }

private:
    RandomValuesGenerator() = default;
    static RandomValuesGenerator _instance;
    std::mt19937 _rng {RandomSeedProvider::Get()};
};




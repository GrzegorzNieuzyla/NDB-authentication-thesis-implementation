#pragma once
#include <cstddef>
#include <chrono>
#include <iostream>
#include <memory>
class RandomSeedProvider
{
public:
    static std::size_t Get()
    {
        static RandomSeedProvider instance {false};
        return instance._seed;
    }
private:
    explicit RandomSeedProvider(bool debug) noexcept : RandomSeedProvider(std::chrono::system_clock::now().time_since_epoch().count(), debug) {}
    explicit RandomSeedProvider(std::size_t seed, bool debug) noexcept : _seed(seed)
    {
        if (debug)
        {
            std::cout << "Using seed: " << seed << std::endl;
        }
    }
    std::size_t _seed;
};


#pragma once

struct WalksatResult
{
    long long flips;
    int solved;
    unsigned long long seconds;
};

struct WalksatResult WalksatMain(int argc, char *argv[]);


void Reset();
#pragma once

struct WalksatResult
{
    long long flips;
    int solved;
    unsigned long long solveTime;
    unsigned long long genTime;
};

struct WalksatResult WalksatMain(int argc, char *argv[]);


void Reset();
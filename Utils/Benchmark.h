#pragma once
#include <chrono>
#include "stdlib.h"
#include "stdio.h"
#include "string.h"


class Benchmark
{
public:
    void Start()
    {
        _start = std::chrono::high_resolution_clock::now();
    }

    std::size_t GetElapsedTime()
    {
        return std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - _start).count();
    }

    static std::string GetMemoryUsage()
    {
        FILE* file = fopen("/proc/self/status", "r");
        int result = -1;
        char line[128];

        while (fgets(line, 128, file) != NULL){
            if (strncmp(line, "VmSize:", 7) == 0){
                result = ParseLine(line);
                break;
            }
        }
        fclose(file);
        if (result < 10240)
        {
            return std::to_string(result) + " kB";
        }
        return std::to_string(result/1024) + " MB";
    }
private:
    static int ParseLine(char* line)
    {
        int i = strlen(line);
        const char* p = line;
        while (*p <'0' || *p > '9') p++;
        line[i-3] = '\0';
        i = std::atoi(p);
        return i;
    }
    std::chrono::time_point<std::chrono::system_clock, std::chrono::duration<long int, std::ratio<1, 1000000000>>> _start;
};
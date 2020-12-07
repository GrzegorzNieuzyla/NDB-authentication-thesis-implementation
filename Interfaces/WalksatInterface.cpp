#include "WalksatInterface.h"
#include "../Streams/DimacsFileStream.h"
#include "../CLIInterface.h"
#include <filesystem>
#include <unistd.h>

WalksatResult WalksatInterface::Solve(const NDB &ndb, int noise)
{
    auto filename = TMP_NDB_FILE + std::to_string(getpid());
    {
        DimacsFileStream stream(filename);
        for (const auto& record : ndb.Records())
        {
            stream << record.ToString() << "\n";
        }
        stream.WriteHeader(ndb.Records().begin()->Size(), ndb.Size());
    }
    std::string empty;
    std::string cutoffStr = "-cutoff";
    auto cutoffStrVal = std::to_string(CLIInterface::GetSettings().walksatCutoffLimit);
    std::string noiseStr = "-noise";
    auto noiseVal = std::to_string(noise);
    std::string solStr = "-numsol";
    std::string solVal = "1";
    char* argv[8] = {empty.data(), cutoffStr.data(), cutoffStrVal.data(), noiseStr.data(), noiseVal.data(), solStr.data(), solVal.data(), filename.data()};
    std::clock_t c_start = std::clock();
    auto result = WalksatMain(8, argv);
    std::clock_t c_end = std::clock();
    result.solveTime = (c_end - c_start) / (CLOCKS_PER_SEC / 1000);
    std::filesystem::remove(filename);
    return result;
}

#include "WalksatInterface.h"
#include "../Streams/DimacsFileStream.h"
#include <filesystem>

WalksatResult WalksatInterface::Solve(const NDB &ndb, std::size_t cutoff, int noise)
{
    {
        DimacsFileStream stream(TMP_NDB_FILE);
        for (const auto& record : ndb.Records())
        {
            stream << record.ToString() << "\n";
        }
        stream.WriteHeader(ndb.Records().begin()->Size(), ndb.Size());
    }
    std::string empty;
    std::string cutoffStr = "-cutoff";
    auto cutoffStrVal = std::to_string(cutoff);
    std::string noiseStr = "-noise";
    auto noiseVal = std::to_string(noise);
    char* argv[6] = {empty.data(), cutoffStr.data(), cutoffStrVal.data(), noiseStr.data(), noiseVal.data(), TMP_NDB_FILE.data()};
    std::clock_t c_start = std::clock();
    auto result = WalksatMain(6, argv);
    std::clock_t c_end = std::clock();
    result.seconds = (c_end-c_start) / CLOCKS_PER_SEC;
    std::filesystem::remove(TMP_NDB_FILE);
    return result;
}

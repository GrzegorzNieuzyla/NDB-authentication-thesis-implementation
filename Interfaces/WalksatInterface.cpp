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
    auto cutoffStr = "-cutoff " + std::to_string(cutoff);
    auto noiseStr = "-noise " + std::to_string(noise);
    char* argv[4] = {empty.data(), cutoffStr.data(), noiseStr.data(), TMP_NDB_FILE.data()};
    auto result = WalksatMain(2, argv);
    std::filesystem::remove(TMP_NDB_FILE);
    return result;
}

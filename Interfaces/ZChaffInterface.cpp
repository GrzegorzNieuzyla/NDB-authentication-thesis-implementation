#include <ctime>
#include "ZChaffInterface.h"
#include "../CLIInterface.h"

ZChaffInterface::ZChaffInterface()
{
    _manager = SAT_InitManager();
}

ZChaffInterface::~ZChaffInterface()
{
    SAT_ReleaseManager(_manager);
}

ZChaffInterface::Result ZChaffInterface::Solve(const NDB &ndb)
{
    Result result {.record = {}, .decisions = 0};
    if (!ndb.Size()) return result;
    SAT_SetNumVariables(_manager, ndb.Records().begin()->Size());
    for (const auto& record : ndb.Records())
    {
        std::vector<int> clause;
        unsigned var = 1;
        for (auto ch : record.Characters())
        {
            if (ch == NDBChar::Bit0)
                clause.push_back(var << 1u);
            else if (ch == NDBChar::Bit1)
                clause.push_back((var << 1u) + 1);
            var++;
        }
        SAT_AddClause(_manager, clause.data(), clause.size());
    }
    SAT_SetTimeLimit(_manager, CLIInterface::GetSettings().zchaffTimeLimit);
    std::clock_t c_start = std::clock();
    auto satResult = SAT_Solve(_manager);
    std::clock_t c_end = std::clock();
    result.seconds = (c_end-c_start) / (CLOCKS_PER_SEC / 1000);
    if (satResult == SATISFIABLE)
    {
        result.record = DBRecord();
        for (int i = 1; i<= SAT_NumVariables(_manager); ++i)
        {
            result.record->Characters().push_back(SAT_GetVarAsgnment(_manager, i) == 1);
        }
    }
    result.decisions = SAT_NumDecisions(_manager);
    return result;
}

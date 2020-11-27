#include "ZChaffInterface.h"

ZChaffInterface::ZChaffInterface()
{
    _manager = SAT_InitManager();
}

ZChaffInterface::~ZChaffInterface()
{
    SAT_ReleaseManager(_manager);
}

std::optional<DBRecord> ZChaffInterface::Solve(const NDB &ndb)
{
    if (!ndb.Size()) return {};
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

    auto result = SAT_Solve(_manager);

    if (result == SATISFIABLE)
    {
        DBRecord record;
        for (int i = 1; i<= SAT_NumVariables(_manager); ++i)
        {
            record.Characters().push_back(SAT_GetVarAsgnment(_manager, i) == 1);
        }
        return record;
    }
    return {};
}

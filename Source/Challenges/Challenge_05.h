#pragma once

#include <Challenges/Common/Challenge.h>
#include <map>
#include <set>

using namespace std;

class CChallenge_05 : public CChallenge
{
private:
    virtual EErrorCode SetUp_FirstPart() override;
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override;

    virtual EErrorCode SetUp_SecondPart() override;
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override;

    bool MustPrecede(const int& pageA, const int& pageB) const;

    static string const sm_inputFilePath;

    typedef map<int, set<int>> PageOrderingRuleSets;
    PageOrderingRuleSets m_MustPrecedeOrderingRules;
    PageOrderingRuleSets m_MustFollowOrderingRules;

    typedef vector<int> PageUpdate;
    vector<PageUpdate> m_PageUpdates;
};

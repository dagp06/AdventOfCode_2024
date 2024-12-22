#pragma once

#include <Challenges/Common/Challenge.h>

#include <set>

using namespace std;

class CChallenge_01 : public CChallenge
{
private:
    virtual EErrorCode SetUp_FirstPart() override;
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override;

    virtual EErrorCode SetUp_SecondPart() override;
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override;

    static string const sm_inputFilePath;

    multiset<int> m_LeftSet;
    multiset<int> m_RightSet;
};

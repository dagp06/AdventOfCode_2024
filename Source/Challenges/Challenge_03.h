#pragma once

#include <Challenges/Common/Challenge.h>

using namespace std;

class CChallenge_03 : public CChallenge
{
private:
    virtual EErrorCode SetUp_FirstPart() override;
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override;

    virtual EErrorCode SetUp_SecondPart() override;
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override;

    bool ProcessMul(const string& line, int& i, long long& totalSum) const;
    bool ProcessPattern(const string& line, int& i, const string& pattern) const;

    static string const sm_inputFilePath;

    vector<string> m_Lines;
};

#pragma once

#include "Common/Challenge.h"

using namespace std;

class CChallenge_04 : public CChallenge
{
private:
    virtual EErrorCode SetUp_FirstPart() override;
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override;

    virtual EErrorCode SetUp_SecondPart() override;
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override;

    static string const sm_inputFilePath;

    vector<string> m_WordSearch;
    int CountWord(const string& word) const;
    int CountPattern(const vector<string>& pattern) const;
    bool ValidateRowCol(int row, int col) const;
};

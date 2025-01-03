#pragma once

#include <Challenges/Common/Challenge.h>
#include <Helpers/Vector2.h>

#include <map>

using namespace std;

class CChallenge_08 : public CChallenge
{
private:
    virtual EErrorCode SetUp_FirstPart() override;
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override;

    virtual EErrorCode SetUp_SecondPart() override;
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override;

    bool IsPosInMappedArea(const Vector2& pos) const;

    static string const sm_inputFilePath;

    typedef map<char, vector<Vector2>> FrequencyPosMap;
    FrequencyPosMap m_FrequencyPos;
    int m_RowCount;
    int m_ColCount;
};

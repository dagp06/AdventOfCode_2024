#pragma once

#include <Challenges/Common/Challenge.h>
#include <Helpers/Vector2.h>

using namespace std;

class CChallenge_18 : public CChallenge
{
private:
    virtual EErrorCode SetUp_FirstPart() override;
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override;

    virtual EErrorCode SetUp_SecondPart() override;
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override;

    long long ComputeShortestPath(const Vector2& _start, const Vector2& _end);
    bool IsPosInMappedArea(const Vector2& _pos) const;

    static string const sm_inputFilePath;
    static const int sm_mapSize = 71;

    vector<Vector2> m_IncomingBytes;
    vector<vector<char>> m_Map;
};

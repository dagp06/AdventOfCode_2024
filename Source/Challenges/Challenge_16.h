#pragma once

#include <Challenges/Common/Challenge.h>
#include <Helpers/Direction.h>
#include <Helpers/Vector2.h>

using namespace std;

class CChallenge_16 : public CChallenge
{
private:
    typedef vector<Vector2> Path;

    virtual EErrorCode SetUp_FirstPart() override;
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override;

    virtual EErrorCode SetUp_SecondPart() override;
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override;

    long long ComputeShortestPath(const Vector2& _start, const Vector2& _end);
    void FindShortestPaths(const Vector2& _start, const Vector2& _end, vector<Path>& _paths);

    static string const sm_inputFilePath;

    Vector2 m_ReindeerPos;
    Vector2 m_EndPos;
    vector<vector<char>> m_Map;
};

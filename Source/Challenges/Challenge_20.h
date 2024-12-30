#pragma once

#include <Challenges/Common/Challenge.h>
#include <Helpers/Vector2.h>

#include <map>

using namespace std;

class CChallenge_20 : public CChallenge
{
private:
    typedef vector<int> Row;
    typedef vector<Row> Map;
    // Key - Picoseconds saved | Value - Cheat Count
    typedef map<int, int> CheatMap;

    virtual EErrorCode SetUp_FirstPart() override;
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override;

    virtual EErrorCode SetUp_SecondPart() override;
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override;

    void ComputeCheatMap_Part1(CheatMap& _cheatMap, const int& _minTimeSaved);
    void ComputeCheatMap_Part2(CheatMap& _cheatMap, const int& _minTimeSaved, const int& _maxCheatDuration);
    bool IsPosInMappedArea(const Vector2& _pos) const;
    bool IsWall(const Vector2& _pos) const;

    static string const sm_inputFilePath;

    Map m_CostMap;
    Vector2 m_Start;
    Vector2 m_End;

#ifdef _DEBUG
    void DrawCostMap() const;
    int m_HighestCost = -1;
#endif // _DEBUG

};

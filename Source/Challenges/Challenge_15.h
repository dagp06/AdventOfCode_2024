#pragma once

#include <Challenges/Common/Challenge.h>
#include <Helpers/Direction.h>
#include <Helpers/Vector2.h>

#include <queue>

using namespace std;

class CChallenge_15 : public CChallenge
{
private:
    virtual EErrorCode SetUp_FirstPart() override;
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override;

    virtual EErrorCode SetUp_SecondPart() override;
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override;
    
    const char& GetMapChar(const Vector2& _pos) const;
    char& GetMapChar(const Vector2& _pos);
    void MoveRobot_Part1(const Direction& _dir);
    void MoveRobot_Part2(const Direction& _dir);
    bool TryMove(const Vector2& _currentPos, const Direction& _dir, queue<Vector2>& _posToTest, vector<Vector2>& _rocksToMove, Vector2Set& _visitedPos);
    void DrawMap() const;

    static string const sm_inputFilePath;

    Vector2 m_RobotPos;
    vector<vector<char>> m_Map;
    vector<Direction> m_Directions;
};

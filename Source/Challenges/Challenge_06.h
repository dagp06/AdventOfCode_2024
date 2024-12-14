#pragma once

#include "Common/Challenge.h"

#include <set>

using namespace std;

class CChallenge_06 : public CChallenge
{
private:
    enum class PosState
    {
        Free,
        Obstacle,
        Visited
    };

    enum class Direction
    {
        Up,
        Right,
        Down,
        Left,
        Count
    };

    struct Position
    {
        Position(int _x = 0, int _y = 0)
            : x(_x), y(_y) {}
        bool operator==(const Position& _other) const { return x == _other.x && y == _other.y; }
        bool operator<(const Position& _other) const { return x < _other.x || (x == _other.x && y < _other.y); }
        
        int x;
        int y;
    };

    struct MapPosition
    {
        public:
            MapPosition(const Position& _pos) : m_Pos(_pos) {}

            void Visit(Direction _incomingDirection) 
            { 
                m_VisitedDirections.insert(_incomingDirection);
            }

            bool WasVisited() const
            {
                return !m_VisitedDirections.empty();
            }

            bool WasVisited(Direction _incomingDirection) const
            {
                return m_VisitedDirections.find(_incomingDirection) != m_VisitedDirections.end();
            }

            Position m_Pos;
            set<Direction> m_VisitedDirections;
            bool m_IsObstacle = false;
    };

    virtual EErrorCode SetUp_FirstPart() override;
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override;

    virtual EErrorCode SetUp_SecondPart() override;
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override;

    bool IsPosInMappedArea_1(const Position& pos) const;
    bool IsPosInMappedArea_2(const Position& pos) const;

    void TestPosAsObstacle(const Position& _obstaclePos, const Position& _guardNextPos, const Direction& _guardDirection);
    void DrawMap_1(const Position& _guardPos, const Position& _debugObstaclePos = Position(-1, -1)) const;
    void DrawMap_2(const vector<vector<MapPosition>>& _map, const Position& _guardPos, const Position& _debugObstaclePos = Position(-1, -1)) const;

    static string const sm_inputFilePath;

    vector<vector<PosState>> m_Map;

    vector<vector<MapPosition>> m_MapPositions;
    Position m_GuardPos;

    set<Position> m_PosWhereObstacleWouldCauseLoop;
    set<Position> m_TestedObstaclePos;
};

#pragma once

#include <Challenges/Common/Challenge.h>
#include <Helpers/Direction.h>
#include <Helpers/Vector2.h>

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

    struct MapPosition
    {
        public:
            MapPosition(const Vector2& _pos) : m_Pos(_pos) {}

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

            Vector2 m_Pos;
            set<Direction> m_VisitedDirections;
            bool m_IsObstacle = false;
    };

    virtual EErrorCode SetUp_FirstPart() override;
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override;

    virtual EErrorCode SetUp_SecondPart() override;
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override;

    bool IsPosInMappedArea_1(const Vector2& pos) const;
    bool IsPosInMappedArea_2(const Vector2& pos) const;

    void TestPosAsObstacle(const Vector2& _obstaclePos, const Vector2& _guardNextPos, const Direction& _guardDirection);
    void DrawMap_1(const Vector2& _guardPos, const Vector2& _debugObstaclePos = Vector2(-1, -1)) const;
    void DrawMap_2(const vector<vector<MapPosition>>& _map, const Vector2& _guardPos, const Vector2& _debugObstaclePos = Vector2(-1, -1)) const;

    static string const sm_inputFilePath;

    vector<vector<PosState>> m_Map;

    vector<vector<MapPosition>> m_MapPositions;
    Vector2 m_GuardPos;

    set<Vector2> m_PosWhereObstacleWouldCauseLoop;
    set<Vector2> m_TestedObstaclePos;
};

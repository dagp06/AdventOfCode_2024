#pragma once

#include "Common/Challenge.h"
#include "../Helpers/Position.h"

#include <unordered_set>

using namespace std;

class CChallenge_12 : public CChallenge
{
private:
    enum class Direction
    {
        Up,
        Right,
        Down,
        Left,
        Count
    };

    struct Edge
    {
        bool operator==(const Edge& _other) const 
        { 
            if (m_DirectionEvaluatedFrom != _other.m_DirectionEvaluatedFrom || m_Positions.size() != _other.m_Positions.size())
                return false;

            for (const Position& pos : m_Positions)
            {
                if (_other.m_Positions.find(pos) == _other.m_Positions.end())
                    return false;
            }

            return true;
        }


        Direction m_DirectionEvaluatedFrom;
        PositionSet m_Positions;
    };

    struct EdgeHash
    {
        size_t operator()(const Edge& _edge) const
        {
            size_t h1 = hash<int>()((int)_edge.m_DirectionEvaluatedFrom);
            size_t h2 = 0;

            for (const Position& pos : _edge.m_Positions)
            {
                h2 ^= PositionHash()(pos) + 0x9e3779b9 + (h2 << 6) + (h2 >> 2);
            }

            return h1 ^ (h2 << 1);
        }
    };

    typedef unordered_set<Edge, EdgeHash> EdgeSet;


    virtual EErrorCode SetUp_FirstPart() override;
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override;

    virtual EErrorCode SetUp_SecondPart() override;
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override;

    long long ComputeFencingPrice_Part1(const char& _plantType, const Position _plantPos);
    void ComputeRegion_Part1(const char& _plantType, const Position _plantPos, int& _regionArea, int& _regionPerimeter);

    long long ComputeFencingPrice_Part2(const char& _plantType, const Position _plantPos);
    void ComputeRegion_Part2(const char& _plantType, const Position _plantPos, const Direction& _incomingDir, int& _regionArea, EdgeSet& _regionEdges);
    void ComputeEdge(const char& _plantType, const Position _plantPos, const Direction& _incomingDir, EdgeSet& _regionEdges);

    bool IsPosInMappedArea(const Position& _pos) const;
    bool IsPosInRegion(const Position& _pos, const char& _regionChar) const;

    static string const sm_inputFilePath;
    static Position const sm_directionDeltas[(int)Direction::Count];

    typedef string GardenRow;
    typedef vector<GardenRow> Garden;
    Garden m_Garden;

    PositionSet m_UnprocessedPlants;
};

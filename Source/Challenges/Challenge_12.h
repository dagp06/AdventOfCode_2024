#pragma once

#include <Challenges/Common/Challenge.h>
#include <Helpers/Direction.h>
#include <Helpers/Vector2.h>

#include <unordered_set>

using namespace std;

class CChallenge_12 : public CChallenge
{
private:
    struct Edge
    {
        bool operator==(const Edge& _other) const 
        { 
            if (m_DirectionEvaluatedFrom != _other.m_DirectionEvaluatedFrom || m_Positions.size() != _other.m_Positions.size())
                return false;

            for (const Vector2& pos : m_Positions)
            {
                if (_other.m_Positions.find(pos) == _other.m_Positions.end())
                    return false;
            }

            return true;
        }


        Direction m_DirectionEvaluatedFrom;
        Vector2Set m_Positions;
    };

    struct EdgeHash
    {
        size_t operator()(const Edge& _edge) const
        {
            size_t h1 = hash<int>()((int)_edge.m_DirectionEvaluatedFrom);
            size_t h2 = 0;

            for (const Vector2& pos : _edge.m_Positions)
            {
                h2 ^= Vector2Hash()(pos) + 0x9e3779b9 + (h2 << 6) + (h2 >> 2);
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

    long long ComputeFencingPrice_Part1(const char& _plantType, const Vector2 _plantPos);
    void ComputeRegion_Part1(const char& _plantType, const Vector2 _plantPos, int& _regionArea, int& _regionPerimeter);

    long long ComputeFencingPrice_Part2(const char& _plantType, const Vector2 _plantPos);
    void ComputeRegion_Part2(const char& _plantType, const Vector2 _plantPos, const Direction& _incomingDir, int& _regionArea, EdgeSet& _regionEdges);
    void ComputeEdge(const char& _plantType, const Vector2 _plantPos, const Direction& _incomingDir, EdgeSet& _regionEdges);

    bool IsPosInMappedArea(const Vector2& _pos) const;
    bool IsPosInRegion(const Vector2& _pos, const char& _regionChar) const;

    static string const sm_inputFilePath;

    typedef string GardenRow;
    typedef vector<GardenRow> Garden;
    Garden m_Garden;

    Vector2Set m_UnprocessedPlants;
};

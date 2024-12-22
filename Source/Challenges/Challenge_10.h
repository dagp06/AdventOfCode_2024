#pragma once

#include "Common/Challenge.h"

#include <unordered_set>
#include <functional>

using namespace std;

class CChallenge_10 : public CChallenge
{
public:

private:
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
        Position operator+(const Position& _other) const { return Position(x + _other.x, y + _other.y); }

        int x;
        int y;
    };

    struct PositionHash {
        size_t operator()(const Position& _pos) const {
            return hash<int>()(_pos.x) ^ (hash<int>()(_pos.y) << 1);
        }
    };

    typedef unordered_set<Position, PositionHash> PositionSet;

    virtual EErrorCode SetUp_FirstPart() override;
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override;

    virtual EErrorCode SetUp_SecondPart() override;
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override;

    void FindHikingTrail(const Position& _currentPos, int _targetHeight, PositionSet& _trailPeaks, long long& _totalScore, bool _uniqueOnly);
    bool IsPosInMappedArea(const Position& pos) const;

    static string const sm_inputFilePath;
    static Position const sm_directionDeltas[(int)Direction::Count];


    typedef vector<vector<int>> Map;
    Map m_Map;
    PositionSet m_TrailHeads;
};

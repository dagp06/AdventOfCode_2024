#pragma once

#include <Challenges/Common/Challenge.h>
#include <Helpers/Vector2.h>

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

    virtual EErrorCode SetUp_FirstPart() override;
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override;

    virtual EErrorCode SetUp_SecondPart() override;
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override;

    void FindHikingTrail(const Vector2& _currentPos, int _targetHeight, Vector2Set& _trailPeaks, long long& _totalScore, bool _uniqueOnly);
    bool IsPosInMappedArea(const Vector2& pos) const;

    static string const sm_inputFilePath;
    static Vector2 const sm_directionDeltas[(int)Direction::Count];


    typedef vector<vector<int>> Map;
    Map m_Map;
    Vector2Set m_TrailHeads;
};

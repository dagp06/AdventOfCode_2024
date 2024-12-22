#include "Challenge_10.h"
#include "../Helpers/FileHelpers.h"



////////////////////////////////////////////////////////////////////////////////////////////////////
string const CChallenge_10::sm_inputFilePath = "Inputs/Input_Challenge_10.txt";
Position const CChallenge_10::sm_directionDeltas[(int)Direction::Count] = { {0, -1}, {1, 0}, {0, 1}, {-1, 0} };


////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_10::SetUp_FirstPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    m_Map.reserve(lines.size());
    for (int i = 0; i < lines.size(); ++i)
    {
        const string& line = lines[i];
        m_Map.push_back(vector<int>());
        vector<int>& row = m_Map.back();

        row.reserve(line.size());
        for (int j = 0; j < line.size(); ++j)
        {
            const char& c = line[j];
            row.push_back(c - '0');

            if (row.back() == 0)
            {
                m_TrailHeads.insert(Position(j, i));
            }
        }
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_10::Run_FirstPart()
{
    long long totalScore = 0ll;

    for (const Position& trailHead : m_TrailHeads)
    {
        PositionSet trailPeaks;
        FindHikingTrail(trailHead, 0, trailPeaks, totalScore, true);
    }

    cout << "The total score for this map is " << totalScore << "." << endl;

    return EErrorCode::Success;
}

EErrorCode CChallenge_10::CleanUp_FirstPart()
{
    m_Map.clear();
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_10::SetUp_SecondPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    m_Map.reserve(lines.size());
    for (int i = 0; i < lines.size(); ++i)
    {
        const string& line = lines[i];
        m_Map.push_back(vector<int>());
        vector<int>& row = m_Map.back();

        row.reserve(line.size());
        for (int j = 0; j < line.size(); ++j)
        {
            const char& c = line[j];
            row.push_back(c - '0');

            if (row.back() == 0)
            {
                m_TrailHeads.insert(Position(j, i));
            }
        }
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_10::Run_SecondPart()
{
    long long totalScore = 0ll;

    for (const Position& trailHead : m_TrailHeads)
    {
        PositionSet trailPeaks;
        FindHikingTrail(trailHead, 0, trailPeaks, totalScore, false);
    }

    cout << "The total score for this map is " << totalScore << "." << endl;

    return EErrorCode::Success;
}

EErrorCode CChallenge_10::CleanUp_SecondPart()
{
    m_Map.clear();
    return EErrorCode::Success;
}


void CChallenge_10::FindHikingTrail(const Position& _currentPos, int _targetHeight, PositionSet& _trailPeaks, long long& _totalScore, bool _uniqueOnly)
{
    if (!IsPosInMappedArea(_currentPos))
        return;

    const int& currentHeight = m_Map[_currentPos.y][_currentPos.x];
    if (currentHeight != _targetHeight)
        return;

    if (currentHeight == 9)
    {
        if (!_uniqueOnly || _trailPeaks.find(_currentPos) == _trailPeaks.end())
        {
            _trailPeaks.insert(_currentPos);
            ++_totalScore;
        }
        return;
    }

    for (Direction dir = Direction::Up; dir != Direction::Count; )
    {
        FindHikingTrail(_currentPos + sm_directionDeltas[(int)dir], _targetHeight + 1, _trailPeaks, _totalScore, _uniqueOnly);

        dir = (Direction)((int)dir + 1);
    }
}

bool CChallenge_10::IsPosInMappedArea(const Position& pos) const
{
    return pos.x >= 0 && pos.x < m_Map[0].size() &&
        pos.y >= 0 && pos.y < m_Map.size();
}

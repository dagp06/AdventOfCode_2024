#include <Challenges/Challenge_20.h>
#include <Helpers/Direction.h>
#include <Helpers/FileHelpers.h>

#include <queue>

#ifdef _DEBUG
#include <fstream>
#endif // _DEBUG


////////////////////////////////////////////////////////////////////////////////////////////////////
string const CChallenge_20::sm_inputFilePath = "Inputs/Input_Challenge_20.txt";





////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_20::SetUp_FirstPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    m_CostMap.reserve(lines.size());
    for (int rowIdx = 0; rowIdx < lines.size(); ++rowIdx)
    {
        const string& line = lines[rowIdx];
        m_CostMap.emplace_back(line.size());
        Row& row = m_CostMap.back();
        for (int colIdx = 0; colIdx < row.size(); ++colIdx)
        {
            switch (line[colIdx])
            {
            case '#':
                row[colIdx] = -1;
                break;
            case '.':
                row[colIdx] = INT32_MAX;
                break;
            case 'S':
                m_Start = Vector2(colIdx, rowIdx);
                row[colIdx] = INT32_MAX;
                break;
            case 'E':
                m_End = Vector2(colIdx, rowIdx);
                row[colIdx] = INT32_MAX;
                break;
            default:
                break;
            }
            row[colIdx] = line[colIdx] == '#' ? -1 : INT32_MAX;
        }
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_20::Run_FirstPart()
{
    CheatMap cheatMap;
    const int minTimeSaved = 100;
    ComputeCheatMap_Part1(cheatMap, minTimeSaved);

#ifdef _DEBUG
    DrawCostMap();
#endif // _DEBUG

    int cheatCount = 0;
    for (auto cheatMapPair : cheatMap)
    {
        cheatCount += cheatMapPair.second;
    }

#ifndef BENCHMARK
    cout << "There are " << cheatCount << " cheats that would save at least " << minTimeSaved << " picoseconds.";
#endif // BENCHMARK

    return EErrorCode::Success;
}

EErrorCode CChallenge_20::CleanUp_FirstPart()
{
    m_Start = Vector2();
    m_End = Vector2();
    m_CostMap.clear();

#ifdef _DEBUG
    m_HighestCost = -1;
#endif // _DEBUG

    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_20::SetUp_SecondPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    m_CostMap.reserve(lines.size());
    for (int rowIdx = 0; rowIdx < lines.size(); ++rowIdx)
    {
        const string& line = lines[rowIdx];
        m_CostMap.emplace_back(line.size());
        Row& row = m_CostMap.back();
        for (int colIdx = 0; colIdx < row.size(); ++colIdx)
        {
            switch (line[colIdx])
            {
            case '#':
                row[colIdx] = -1;
                break;
            case '.':
                row[colIdx] = INT32_MAX;
                break;
            case 'S':
                m_Start = Vector2(colIdx, rowIdx);
                row[colIdx] = INT32_MAX;
                break;
            case 'E':
                m_End = Vector2(colIdx, rowIdx);
                row[colIdx] = INT32_MAX;
                break;
            default:
                break;
            }
            row[colIdx] = line[colIdx] == '#' ? -1 : INT32_MAX;
        }
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_20::Run_SecondPart()
{
    CheatMap cheatMap;
    const int minTimeSaved = 100;
    const int maxCheatDuration = 20;
    ComputeCheatMap_Part2(cheatMap, minTimeSaved, maxCheatDuration);

#ifdef _DEBUG
    DrawCostMap();
#endif // _DEBUG

    int cheatCount = 0;
    for (auto cheatMapPair : cheatMap)
    {
        cheatCount += cheatMapPair.second;
    }

#ifndef BENCHMARK
    cout << "There are " << cheatCount << " cheats of " << maxCheatDuration << " picoseconds or less that would save at least " << minTimeSaved << " picoseconds.";
#endif // BENCHMARK

    return EErrorCode::Success;
}

EErrorCode CChallenge_20::CleanUp_SecondPart()
{
    m_Start = Vector2();
    m_End = Vector2();
    m_CostMap.clear();

#ifdef _DEBUG
    m_HighestCost = -1;
#endif // _DEBUG

    return EErrorCode::Success;
}


void CChallenge_20::ComputeCheatMap_Part1(CheatMap& _cheatMap, const int& _minTimeSaved)
{
#ifdef _DEBUG
    m_HighestCost = -1;
#endif // _DEBUG

    queue<Vector2> posToVisit;
    posToVisit.push({}); // Dummy
    posToVisit.push(m_End);
    m_CostMap[m_End.y][m_End.x] = 0;

    while (posToVisit.size() > 1)
    {
        posToVisit.pop();
        const Vector2& pos = posToVisit.front();

        for (Direction dir = Direction::Up; dir != Direction::Count; )
        {
            Vector2 newPos = pos + DirectionUtils::GetDelta(dir);

            if (!IsPosInMappedArea(newPos))
                continue;

            // Compute cheat map
            if (IsWall(newPos))
            {
                // Check if there's a free space behind it that was already traversed
                newPos += DirectionUtils::GetDelta(dir);
                if (IsPosInMappedArea(newPos) && !IsWall(newPos) && m_CostMap[newPos.y][newPos.x] != INT32_MAX)
                {
                    // If so, compute the time saved if we passed through the wall
                    int timeSaved = m_CostMap[pos.y][pos.x] - m_CostMap[newPos.y][newPos.x] - 2;
                    if (timeSaved >= _minTimeSaved)
                        ++_cheatMap[timeSaved];
                }
            }
            // Compute cost map
            else if (posToVisit.size() == 1)
            {
                int newCost = m_CostMap[pos.y][pos.x] + 1;
                int& currentCost = m_CostMap[newPos.y][newPos.x];
                if (newCost < currentCost)
                {
                    currentCost = newCost;
                    posToVisit.push(newPos);

#ifdef _DEBUG
                    m_HighestCost = max(m_HighestCost, newCost);
#endif // _DEBUG
                }
            }

            dir = DirectionUtils::GetNext(dir, false);
        }
    }
}


void CChallenge_20::ComputeCheatMap_Part2(CheatMap& _cheatMap, const int& _minTimeSaved, const int& _maxCheatDuration)
{
#ifdef _DEBUG
    m_HighestCost = -1;
#endif // _DEBUG

    queue<Vector2> posToVisit;
    posToVisit.push({}); // Dummy
    posToVisit.push(m_End);
    m_CostMap[m_End.y][m_End.x] = 0;

    while (posToVisit.size() > 1)
    {
        posToVisit.pop();
        const Vector2& pos = posToVisit.front();

        int currentCost = m_CostMap[pos.y][pos.x];
        if (currentCost > 3)
        {
            int minX = max((int)pos.x -_maxCheatDuration, 0);
            int maxX = min((int)m_CostMap[0].size(), (int)pos.x + _maxCheatDuration);
            for (int explorationX = minX; explorationX < maxX + 1; ++explorationX)
            {
                int distX = abs(explorationX - (int)pos.x);
                int rangeY = _maxCheatDuration - distX;
                int minY = max((int)pos.y - rangeY, 0);
                int maxY = min((int)m_CostMap.size(), (int)pos.y + rangeY);
                for (int explorationY = minY; explorationY < maxY + 1; ++explorationY)
                {
                    Vector2 newPos = Vector2(explorationX, explorationY);
                    if (IsPosInMappedArea(newPos) && !IsWall(newPos))
                    {
                        const int& newPosCost = m_CostMap[newPos.y][newPos.x];
                        if (newPosCost != INT32_MAX)
                        {
                            int dist = distX + abs(explorationY - (int)pos.y);
                            int timeSaved = currentCost - m_CostMap[newPos.y][newPos.x] - dist;
                            if (timeSaved >= _minTimeSaved)
                                ++_cheatMap[timeSaved];
                        }
                    }
                }
            }
        }

        for (Direction dir = Direction::Up; dir != Direction::Count; )
        {
            Vector2 newPos = pos + DirectionUtils::GetDelta(dir);

            // Compute cost map
            if (IsPosInMappedArea(newPos) && !IsWall(newPos))
            {
                int newCost = m_CostMap[pos.y][pos.x] + 1;
                int& currentCost = m_CostMap[newPos.y][newPos.x];
                if (newCost < currentCost)
                {
                    currentCost = newCost;
                    posToVisit.push(newPos);

#ifdef _DEBUG
                    m_HighestCost = max(m_HighestCost, newCost);
#endif // _DEBUG
                    break;
                }
            }

            dir = DirectionUtils::GetNext(dir, false);
        }
    }
}

bool CChallenge_20::IsPosInMappedArea(const Vector2& _pos) const
{
    return _pos.x >= 0 && _pos.x < (int)m_CostMap[0].size() &&
        _pos.y >= 0 && _pos.y < (int)m_CostMap.size();
}

bool CChallenge_20::IsWall(const Vector2& _pos) const
{
    return m_CostMap[_pos.y][_pos.x] == -1;
}

#ifdef _DEBUG
void CChallenge_20::DrawCostMap() const
{
    std::ofstream outFile("C20_map.txt");
    if (outFile.is_open())
    {
        int blockSize = (int)to_string(m_HighestCost).size() + 1;

        for (int rowIdx = 0; rowIdx < m_CostMap.size(); ++rowIdx)
        {
            const Row& row = m_CostMap[rowIdx];
            for (int colIdx = 0; colIdx < row.size(); ++colIdx)
            {
                const int& cost = row[colIdx];
                if (cost == -1)
                {
                    outFile << "#" << string(blockSize - 1, ' ');
                }
                else if (cost == INT32_MAX)
                {
                    outFile << "." << string(blockSize - 1, ' ');
                }
                else
                {
                    outFile << row[colIdx] << string(blockSize - to_string(row[colIdx]).size(), ' ');
                }
            }
            outFile << endl;
        }
        outFile << endl;
    }
}
#endif // _DEBUG
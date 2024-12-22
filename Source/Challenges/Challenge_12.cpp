#include "Challenge_12.h"
#include "../Helpers/FileHelpers.h"



////////////////////////////////////////////////////////////////////////////////////////////////////
string const CChallenge_12::sm_inputFilePath = "Inputs/Input_Challenge_12.txt";
CChallenge_12::Position const CChallenge_12::sm_directionDeltas[(int)Direction::Count] = { {0, -1}, {1, 0}, {0, 1}, {-1, 0} };





////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_12::SetUp_FirstPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    m_Garden.reserve(lines.size());
    for (int i = 0; i < lines.size(); ++i)
    {
        const string& line = lines[i];
        m_Garden.push_back(line);

        for (int j = 0; j < line.size(); ++j)
        {
            m_UnprocessedPlants.insert(Position(j, i));
        }
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_12::Run_FirstPart()
{
    long long totalFencingPrice = 0ll;

    PositionSet::const_iterator plantToProcessIt = m_UnprocessedPlants.cbegin();
    while (plantToProcessIt != m_UnprocessedPlants.cend())
    {
        const Position& plantPos = *(plantToProcessIt);
        const char& plantType = m_Garden[plantPos.y][plantPos.x];

        totalFencingPrice += ComputeFencingPrice_Part1(plantType, plantPos);

        plantToProcessIt = m_UnprocessedPlants.begin();
    }

    cout << "The total price to fence all regions is " << totalFencingPrice << "." << endl;

    return EErrorCode::Success;
}

EErrorCode CChallenge_12::CleanUp_FirstPart()
{
    m_Garden.clear();
    m_UnprocessedPlants.clear();
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_12::SetUp_SecondPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    m_Garden.reserve(lines.size());
    for (int i = 0; i < lines.size(); ++i)
    {
        const string& line = lines[i];
        m_Garden.push_back(line);

        for (int j = 0; j < line.size(); ++j)
        {
            m_UnprocessedPlants.insert(Position(j, i));
        }
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_12::Run_SecondPart()
{
    long long totalFencingPrice = 0ll;

    PositionSet::const_iterator plantToProcessIt = m_UnprocessedPlants.cbegin();
    while (plantToProcessIt != m_UnprocessedPlants.cend())
    {
        const Position& plantPos = *(plantToProcessIt);
        const char& plantType = m_Garden[plantPos.y][plantPos.x];

        totalFencingPrice += ComputeFencingPrice_Part2(plantType, plantPos);

        plantToProcessIt = m_UnprocessedPlants.begin();
    }

    cout << "The total price to fence all regions is " << totalFencingPrice << "." << endl;

    return EErrorCode::Success;
}

EErrorCode CChallenge_12::CleanUp_SecondPart()
{
    m_Garden.clear();
    m_UnprocessedPlants.clear();
    return EErrorCode::Success;
}


long long CChallenge_12::ComputeFencingPrice_Part1(const char& _plantType, const Position _plantPos)
{
    int regionArea = 0;
    int regionPerimeter = 0;
    ComputeRegion_Part1(_plantType, _plantPos, regionArea, regionPerimeter);

    return regionArea * regionPerimeter;
}


void CChallenge_12::ComputeRegion_Part1(const char& _plantType, const Position _plantPos, int& _regionArea, int& _regionPerimeter)
{
    if (!IsPosInRegion(_plantPos, _plantType))
    {
        // Stepped outside the region, add a fence
        ++_regionPerimeter;
        return;
    }

    PositionSet::iterator plantIt = m_UnprocessedPlants.find(_plantPos);
    if (plantIt == m_UnprocessedPlants.end())
    {
        // Already processed this plant
        return;
    }

    // Process this plant
    m_UnprocessedPlants.erase(plantIt);
    ++_regionArea;

    // Visit neighbors
    for (Direction dir = Direction::Up; dir != Direction::Count; )
    {
        ComputeRegion_Part1(_plantType, _plantPos + sm_directionDeltas[(int)dir], _regionArea, _regionPerimeter);

        dir = (Direction)((int)dir + 1);
    }
}


long long CChallenge_12::ComputeFencingPrice_Part2(const char& _plantType, const Position _plantPos)
{
    int regionArea = 0;
    EdgeSet regionEdges;
    ComputeRegion_Part2(_plantType, _plantPos, Direction::Count, regionArea, regionEdges);

    return regionArea * regionEdges.size();
}


void CChallenge_12::ComputeRegion_Part2(const char& _plantType, const Position _plantPos, const Direction& _incomingDir, int& _regionArea, EdgeSet& _regionEdges)
{
    if (!IsPosInRegion(_plantPos, _plantType))
    {
        // Stepped outside the region, add a fence
        Position preStepPos = _plantPos - sm_directionDeltas[((int)_incomingDir)];
        ComputeEdge(_plantType, preStepPos, _incomingDir, _regionEdges);
        return;
    }

    PositionSet::iterator plantIt = m_UnprocessedPlants.find(_plantPos);
    if (plantIt == m_UnprocessedPlants.end())
    {
        // Already processed this plant
        return;
    }

    // Process this plant
    m_UnprocessedPlants.erase(plantIt);
    ++_regionArea;

    // Visit neighbors
    for (Direction dir = Direction::Up; dir != Direction::Count; )
    {
        ComputeRegion_Part2(_plantType, _plantPos + sm_directionDeltas[(int)dir], dir, _regionArea, _regionEdges);

        dir = (Direction)((int)dir + 1);
    }
}

void CChallenge_12::ComputeEdge(const char& _plantType, const Position _plantPos, const Direction& _incomingDir, EdgeSet& _regionEdges)
{
    for (const Edge& edge : _regionEdges)
    {
        if (_incomingDir != edge.m_DirectionEvaluatedFrom)
            continue;

        if (edge.m_Positions.find(_plantPos) != edge.m_Positions.end())
        {
            // Edge was already processed
            return;
        }
    }

    // New edge
    Edge newEdge;
    newEdge.m_DirectionEvaluatedFrom = _incomingDir;
    newEdge.m_Positions.insert(_plantPos); // Insert current pos on edge

    vector<Direction> explorationDirs;
    explorationDirs.reserve(2);
    switch (_incomingDir)
    {
    case Direction::Up:
    case Direction::Down:
        explorationDirs.push_back(Direction::Left);
        explorationDirs.push_back(Direction::Right);
        break;
    case Direction::Left:
    case Direction::Right:
        explorationDirs.push_back(Direction::Up);
        explorationDirs.push_back(Direction::Down);
        break;
    default:
        break;
    }

    Position frontSteppingDelta = sm_directionDeltas[(int)_incomingDir];
    for (int i = 0; i < explorationDirs.size(); ++i)
    {
        Direction explorationDir = explorationDirs[i];
        Position sideSteppingDelta = sm_directionDeltas[(int)explorationDir];

        Position sideSteppingPos = _plantPos + sideSteppingDelta;
        while (IsPosInRegion(sideSteppingPos, _plantType)) // Edge ends if we side-step outside of current region
        {
            Position frontSteppingPos = sideSteppingPos + frontSteppingDelta;
            if (!IsPosInRegion(frontSteppingPos, _plantType))
            {
                // Front-stepped outside the region, still facing the edge
                newEdge.m_Positions.insert(sideSteppingPos); //Insert new pos on edge

                sideSteppingPos = sideSteppingPos + sideSteppingDelta;
            }
            else
            {
                // Front-stepped within region, edge no longer continues in this direction.
                break;
            }
        }
    }

    _regionEdges.insert(newEdge);
}

bool CChallenge_12::IsPosInMappedArea(const Position& _pos) const
{
    return _pos.x >= 0 && _pos.x < m_Garden[0].size() &&
        _pos.y >= 0 && _pos.y < m_Garden.size();
}

bool CChallenge_12::IsPosInRegion(const Position& _pos, const char& _regionChar) const
{
    return IsPosInMappedArea(_pos) && m_Garden[_pos.y][_pos.x] == _regionChar;
}

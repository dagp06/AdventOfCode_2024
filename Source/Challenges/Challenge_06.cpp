#include <Challenges/Challenge_06.h>
#include <Helpers/FileHelpers.h>



////////////////////////////////////////////////////////////////////////////////////////////////////
string const CChallenge_06::sm_inputFilePath = "Inputs/Input_Challenge_06.txt";



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_06::SetUp_FirstPart()
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

        m_Map.emplace_back();
        vector<PosState>& currentRow = m_Map.back();
        currentRow.reserve(line.size());
        for (int j = 0; j < line.size(); ++j)
        {
            const char& ch = line[j];
            switch (ch)
            {
                case '^':
                    m_GuardPos = Vector2(j, i);
                    currentRow.push_back(PosState::Visited);
                    break;

                case '.':
                    currentRow.push_back(PosState::Free);
                    break;

                case '#':
                default:
                    currentRow.push_back(PosState::Obstacle);
                    break;
            }
        }
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_06::Run_FirstPart()
{
    int visitedPos = 1;

    Direction guardDirection = Direction::Up;
    while (true)
    {
        Vector2 nextPosition = m_GuardPos;
        switch (guardDirection)
        {
            case Direction::Up:
                --nextPosition.y;
                break;
            case Direction::Right:
                ++nextPosition.x;
                break;
            case Direction::Down:
                ++nextPosition.y;
                break;
            case Direction::Left:
                --nextPosition.x;
                break;
            default:
                break;
        }

        // We're done !
        if (!IsPosInMappedArea_1(nextPosition))
            break;

        PosState& nextPosState = m_Map[nextPosition.y][nextPosition.x];
        switch (nextPosState)
        {
            case CChallenge_06::PosState::Free:
                ++visitedPos;
                nextPosState = PosState::Visited;
                m_GuardPos = nextPosition;
                continue;

            case CChallenge_06::PosState::Visited:
                m_GuardPos = nextPosition;
                continue;

            case CChallenge_06::PosState::Obstacle:
                // Obstacle encountered, must turn
                guardDirection = static_cast<Direction>((static_cast<int>(guardDirection) + 1) % static_cast<int>(Direction::Count));

            default:
                break;
        }
    }

    cout << "The guard will visit " << visitedPos << " positions before leaving the mapped area." << endl;

#ifdef _DEBUG
    DrawMap_1(m_GuardPos);
#endif

    return EErrorCode::Success;
}

EErrorCode CChallenge_06::CleanUp_FirstPart()
{
    m_Map.clear();
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_06::SetUp_SecondPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    m_MapPositions.reserve(lines.size());
    for (int i = 0; i < lines.size(); ++i)
    {
        const string& line = lines[i];

        m_MapPositions.emplace_back();
        vector<MapPosition>& currentRow = m_MapPositions.back();
        currentRow.reserve(line.size());
        for (int j = 0; j < line.size(); ++j)
        {
            currentRow.emplace_back(Vector2(j, i));
            MapPosition& currentMapPos = currentRow.back();

            const char& ch = line[j];
            switch (ch)
            {
                case '^':
                    m_GuardPos = Vector2(j, i);
                    currentMapPos.Visit(Direction::Up);
                    break;

                case '#':
                    currentMapPos.m_IsObstacle = true;
                    break;

                default:
                    break;
            }
        }
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_06::Run_SecondPart()
{
    Direction guardDirection = Direction::Up;

    // Consider starting pos as already evaluated for potential obstacles, since we can't place one there.
    m_TestedObstaclePos.insert(m_GuardPos);

    while (true)
    {
        Vector2 nextPosition = m_GuardPos;
        switch (guardDirection)
        {
            case Direction::Up:
                --nextPosition.y;
                break;
            case Direction::Right:
                ++nextPosition.x;
                break;
            case Direction::Down:
                ++nextPosition.y;
                break;
            case Direction::Left:
                --nextPosition.x;
                break;
            default:
                break;
        }

        // We're done !
        if (!IsPosInMappedArea_2(nextPosition))
        {
            m_GuardPos = nextPosition;
            break;
        }

        MapPosition& nextMapPos = m_MapPositions[nextPosition.y][nextPosition.x];
        if (nextMapPos.m_IsObstacle)
        {
            // Obstacle encountered, must turn
            guardDirection = static_cast<Direction>((static_cast<int>(guardDirection) + 1) % static_cast<int>(Direction::Count));
        }
        else
        {
            TestPosAsObstacle(nextPosition, m_GuardPos, guardDirection);
            m_GuardPos = nextPosition;
            nextMapPos.Visit(guardDirection);
        }
    }

    cout << "There are " << m_PosWhereObstacleWouldCauseLoop.size() << " positions where obstacles would cause the guard to become stuck in a loop." << endl;

#ifdef _DEBUG
    DrawMap_2(m_MapPositions, m_GuardPos);
#endif

    return EErrorCode::Success;
}

EErrorCode CChallenge_06::CleanUp_SecondPart()
{
    m_MapPositions.clear();
    m_PosWhereObstacleWouldCauseLoop.clear();
    m_TestedObstaclePos.clear();
    return EErrorCode::Success;
}

bool CChallenge_06::IsPosInMappedArea_1(const Vector2& pos) const
{
    return pos.x >= 0ll && pos.x < (long long)m_Map[0].size() &&
        pos.y >= 0ll && pos.y < (long long)m_Map.size();
}

bool CChallenge_06::IsPosInMappedArea_2(const Vector2& pos) const
{
    return pos.x >= 0ll && pos.x < (long long)m_MapPositions[0].size() &&
        pos.y >= 0ll && pos.y < (long long)m_MapPositions.size();
}

void CChallenge_06::TestPosAsObstacle(const Vector2& _obstaclePos, const Vector2& _guardPos, const Direction& _guardDirection)
{
    // We already tested this pos
    if (m_TestedObstaclePos.find(_obstaclePos) != m_TestedObstaclePos.end())
        return;

    // We'll evaluate this pos as a potential obstacle
    m_TestedObstaclePos.insert(_obstaclePos);

    vector<vector<MapPosition>> testMap = m_MapPositions; // Copy map for local test
    testMap[_obstaclePos.y][_obstaclePos.x].m_IsObstacle = true; // Set new pos as obstacle
    Vector2 testPos = _guardPos; // Start in front of the new potential obstacle
    Direction testDirection = static_cast<Direction>((static_cast<int>(_guardDirection) + 1) % static_cast<int>(Direction::Count)); // Turn right
    while (true)
    {
        Vector2 nextPosition = testPos;
        switch (testDirection)
        {
            case Direction::Up:
                --nextPosition.y;
                break;
            case Direction::Right:
                ++nextPosition.x;
                break;
            case Direction::Down:
                ++nextPosition.y;
                break;
            case Direction::Left:
                --nextPosition.x;
                break;
            default:
                break;
        }

        if (!IsPosInMappedArea_2(nextPosition))
        {
            // We exited the mapped area without looping.
            return;
        }

        MapPosition& nextMapPos = testMap[nextPosition.y][nextPosition.x];
        if (nextMapPos.WasVisited(testDirection))
        {
            // Loop detected!
            m_PosWhereObstacleWouldCauseLoop.insert(_obstaclePos);

#ifdef _DEBUG
            cout << m_PosWhereObstacleWouldCauseLoop.size() << "\t-\t" << _obstaclePos.x << "\t" << _obstaclePos.y << endl;
#endif
            return;
        }

        if (nextMapPos.m_IsObstacle)
        {
            // Obstacle encountered, must turn
            testDirection = static_cast<Direction>((static_cast<int>(testDirection) + 1) % static_cast<int>(Direction::Count));
        }
        else
        {
            testPos = nextPosition;
            nextMapPos.Visit(testDirection);
        }
    }
}

void CChallenge_06::DrawMap_1(const Vector2& _guardPos, const Vector2& _debugObstaclePos) const
{
    for (int i = 0; i < m_Map.size(); ++i)
    {
        const vector<PosState>& row = m_Map[i];
        for (int j = 0; j < row.size(); ++j)
        {
            if (_guardPos.x == j && _guardPos.y == i)
            {
                cout << '^';
                continue;
            }

            if (m_PosWhereObstacleWouldCauseLoop.find(Vector2(j, i)) != m_PosWhereObstacleWouldCauseLoop.end())
            {
                cout << "O";
                continue;
            }

            if (_debugObstaclePos == Vector2(j, i))
            {
                cout << "O";
                continue;
            }

            const PosState& posState = row[j];
            switch (posState)
            {
            case CChallenge_06::PosState::Free:
                cout << ".";
                break;
            case CChallenge_06::PosState::Obstacle:
                cout << "#";
                break;
            case CChallenge_06::PosState::Visited:
                cout << "X";
                break;
            default:
                break;
            }
        }
        cout << endl;
    }
    cout << endl;
}


void CChallenge_06::DrawMap_2(const vector<vector<MapPosition>>& _map, const Vector2& _guardPos, const Vector2& _debugObstaclePos /*= Position(-1, -1)*/) const
{
    for (int i = 0; i < _map.size(); ++i)
    {
        const vector<MapPosition>& row = _map[i];
        for (int j = 0; j < row.size(); ++j)
        {
            if (_guardPos.x == j && _guardPos.y == i)
            {
                cout << '^';
                continue;
            }

            if (_debugObstaclePos == Vector2(-1, -1) && m_PosWhereObstacleWouldCauseLoop.find(Vector2(j, i)) != m_PosWhereObstacleWouldCauseLoop.end())
            {
                cout << "O";
                continue;
            }

            if (_debugObstaclePos == Vector2(j, i))
            {
                cout << "0";
                continue;
            }

            const MapPosition& mapPos = row[j];
            if (mapPos.m_IsObstacle)
            {
                cout << "#";
            }
            else
            {
                if (mapPos.WasVisited())
                {
                    cout << "X";
                }
                else
                {
                    cout << ".";
                }
            }
        }
        cout << endl;
    }
    cout << endl;
}

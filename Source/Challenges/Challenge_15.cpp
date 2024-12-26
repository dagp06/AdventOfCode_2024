#include <Challenges/Challenge_15.h>
#include <Helpers/FileHelpers.h>



////////////////////////////////////////////////////////////////////////////////////////////////////
string const CChallenge_15::sm_inputFilePath = "Inputs/Input_Challenge_15.txt";





////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_15::SetUp_FirstPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    const int mapWidth = (int)lines[0].size();

    int i = 0;
    const string* linePtr = &lines[i];
    while (!linePtr->empty())
    {
        const string& line = *linePtr;

        m_Map.emplace_back();
        vector<char>& mapLine = m_Map.back();
        mapLine.reserve(mapWidth);
        for (int j = 0; j < mapWidth; ++j)
        {
            if (line[j] == '@')
            {
                m_RobotPos = { j, i };
                mapLine.push_back('.');
            }
            else
                mapLine.push_back(line[j]);
        }

        linePtr = &lines[++i];
    }

    ++i;
    while (i < lines.size())
    {
        const string& directionLine = lines[i++];
        for (const char& c : directionLine)
        {
            switch (c)
            {
            case '^':
                m_Directions.push_back(Direction::Up);
                break;
            case 'v':
                m_Directions.push_back(Direction::Down);
                break;
            case '<':
                m_Directions.push_back(Direction::Left);
                break;
            case '>':
                m_Directions.push_back(Direction::Right);
                break;
            default:
                break;
            }
        }
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_15::Run_FirstPart()
{
#ifdef _DEBUG
    // Draw starting map
    DrawMap();
    int i = 0;
#endif // _DEBUG

    // Execute robot movements
    for (const Direction& dir : m_Directions)
    {
        MoveRobot_Part1(dir);
#ifdef _DEBUG
        cout << i++ << " - Moving " << dir << ":" << endl;
        DrawMap();
#endif // _DEBUG
    }

    // Compute boxes GPS coordinates sum
    long long boxesGPSCoordSum = 0ll;
    for (int i = 0; i < m_Map.size(); ++i)
    {
        const vector<char>& row = m_Map[i];
        for (int j = 0; j < row.size(); ++j)
        {
            if (row[j] != 'O')
                continue;

            boxesGPSCoordSum += 100ll * i + j;
        }
    }

    cout << "The sum of all boxes' GPS coordinates is " << boxesGPSCoordSum << "." << endl;

    return EErrorCode::Success;
}

EErrorCode CChallenge_15::CleanUp_FirstPart()
{
    m_Map.clear();
    m_Directions.clear();
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_15::SetUp_SecondPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    const int mapWidth = (int)lines[0].size() * 2;

    int i = 0;
    const string* linePtr = &lines[i];
    while (!linePtr->empty())
    {
        const string& line = *linePtr;

        m_Map.emplace_back();
        vector<char>& mapLine = m_Map.back();
        mapLine.reserve(mapWidth);
        for (int j = 0; j < line.size(); ++j)
        {
            if (line[j] == '@')
            {
                m_RobotPos = { 2 * j, i };
                mapLine.push_back('.');
                mapLine.push_back('.');
            }
            else if (line[j] == 'O')
            {
                mapLine.push_back('[');
                mapLine.push_back(']');
            }
            else
            {
                mapLine.push_back(line[j]);
                mapLine.push_back(line[j]);
            }
        }

        linePtr = &lines[++i];
    }

    ++i;
    while (i < lines.size())
    {
        const string& directionLine = lines[i++];
        for (const char& c : directionLine)
        {
            switch (c)
            {
            case '^':
                m_Directions.push_back(Direction::Up);
                break;
            case 'v':
                m_Directions.push_back(Direction::Down);
                break;
            case '<':
                m_Directions.push_back(Direction::Left);
                break;
            case '>':
                m_Directions.push_back(Direction::Right);
                break;
            default:
                break;
            }
        }
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_15::Run_SecondPart()
{
#ifdef _DEBUG
    // Draw starting map
    DrawMap();
    int i = 0;
#endif // _DEBUG

    // Execute robot movements
    for (const Direction& dir : m_Directions)
    {
        MoveRobot_Part2(dir);
#ifdef _DEBUG
        cout << i++ << " - Moving " << dir << ":" << endl;
        DrawMap();
#endif // _DEBUG
    }

    // Compute boxes GPS coordinates sum
    long long boxesGPSCoordSum = 0ll;
    for (int i = 0; i < m_Map.size(); ++i)
    {
        const vector<char>& row = m_Map[i];
        for (int j = 0; j < row.size(); ++j)
        {
            if (row[j] != '[')
                continue;

            boxesGPSCoordSum += 100ll * i + j;
        }
    }

    cout << "The sum of all boxes' GPS coordinates is " << boxesGPSCoordSum << "." << endl;

    return EErrorCode::Success;
}

EErrorCode CChallenge_15::CleanUp_SecondPart()
{
    m_Map.clear();
    m_Directions.clear();
    return EErrorCode::Success;
}


char& CChallenge_15::GetMapChar(const Vector2& _pos)
{
    return m_Map[_pos.y][_pos.x];
}

const char& CChallenge_15::GetMapChar(const Vector2& _pos) const
{
    return m_Map[_pos.y][_pos.x];
}

void CChallenge_15::MoveRobot_Part1(const Direction& _dir)
{
    const Vector2 delta = DirectionUtils::GetDelta(_dir);
    const Vector2 tentativePos = m_RobotPos + delta;
    char& tentativePosChar = GetMapChar(tentativePos);
    if (tentativePosChar == '.')
    {
        m_RobotPos = tentativePos;
        return;
    }
    else if (tentativePosChar == 'O')
    {
        Vector2 nextPos = tentativePos + delta;
        while (m_Map[nextPos.y][nextPos.x] == 'O')
        {
            nextPos += delta;
        }

        char& nextPosChar = GetMapChar(nextPos);
        if (nextPosChar == '.')
        {
            nextPosChar = 'O';
            tentativePosChar = '.';
            m_RobotPos = tentativePos;
        }
    }
}


void CChallenge_15::MoveRobot_Part2(const Direction& _dir)
{
    const Vector2 delta = DirectionUtils::GetDelta(_dir);
    const Vector2 tentativePos = m_RobotPos + delta;
    char& tentativePosChar = GetMapChar(tentativePos);
    if (tentativePosChar == '.')
    {
        m_RobotPos = tentativePos;
        return;
    }
    
    if (tentativePosChar == '#')
        return;

    queue<Vector2> posToTest;
    vector<Vector2> rocksToMove;
    Vector2Set visitedPos;
    Vector2 dirDelta = DirectionUtils::GetDelta(_dir);
    bool moveSucceeded = true;
    posToTest.push(tentativePos);
    while (!posToTest.empty())
    {
        Vector2 pos = posToTest.front();
        posToTest.pop();

        if (!TryMove(pos, _dir, posToTest, rocksToMove, visitedPos))
        {
            moveSucceeded = false;
            break;
        }
    }

    if (moveSucceeded)
    {
        m_RobotPos = tentativePos;

        // Move rocks
        static const Vector2 rightDirDelta = DirectionUtils::GetDelta(Direction::Right);
        for (vector<Vector2>::const_reverse_iterator it = rocksToMove.rbegin(); it != rocksToMove.rend(); ++it)
        {
            GetMapChar(*it) = '.';
            GetMapChar(*it + rightDirDelta) = '.';

            Vector2 newRockPos = *it + dirDelta;
            GetMapChar(newRockPos) = '[';
            GetMapChar(newRockPos + rightDirDelta) = ']';
        }
    }
    
}

bool CChallenge_15::TryMove(const Vector2& _currentPos, const Direction& _dir, queue<Vector2>& _posToTest, vector<Vector2>& _rocksToMove, Vector2Set& _visitedPos)
{
    const char& c = GetMapChar(_currentPos);
    if (c == '.')
        return true;

    if (c == '#')
        return false;

    Vector2 currentRockLeft;
    Vector2 currentRockRight;
    if (c == '[')
    {
        currentRockLeft = _currentPos;
        currentRockRight = _currentPos + DirectionUtils::GetDelta(Direction::Right);
    }
    else // c == ']'
    {
        currentRockRight = _currentPos;
        currentRockLeft = _currentPos + DirectionUtils::GetDelta(Direction::Left);
    }

    if (_visitedPos.find(currentRockLeft) != _visitedPos.end())
        return true;

    _visitedPos.insert(currentRockLeft);
    _rocksToMove.push_back(currentRockLeft);

    switch (_dir)
    {
        case Direction::Up:
        case Direction::Down:
            _posToTest.push(currentRockLeft + DirectionUtils::GetDelta(_dir));
            _posToTest.push(currentRockRight + DirectionUtils::GetDelta(_dir));
            break;
        case Direction::Right:
            _posToTest.push(currentRockRight + DirectionUtils::GetDelta(Direction::Right));
            break;
        case Direction::Left:
            _posToTest.push(currentRockLeft + DirectionUtils::GetDelta(Direction::Left));
            break;
        default:
            break;
    }

    return true;
}

void CChallenge_15::DrawMap() const
{
    // Output content of m_Map
    for (int i = 0; i < m_Map.size(); ++i)
    {
        for (int j = 0; j < m_Map[0].size(); ++j)
        {
            Vector2 pos { j, i };
            if (m_RobotPos == pos)
                cout << '@';
            else
                cout << GetMapChar(pos);
        }
        cout << endl;
    }

    cout << endl;
}

#include <Challenges/Challenge_18.h>
#include <Helpers/Direction.h>
#include <Helpers/FileHelpers.h>

#include <queue>

////////////////////////////////////////////////////////////////////////////////////////////////////
string const CChallenge_18::sm_inputFilePath = "Inputs/Input_Challenge_18.txt";



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_18::SetUp_FirstPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    m_Map.resize(sm_mapSize, vector<char>(sm_mapSize, '.'));

    m_IncomingBytes.reserve(lines.size());
    for (string const& line : lines)
    {
        vector<string> byteStr;
        FileHelper::SplitLine(line, ",", byteStr);
        m_IncomingBytes.emplace_back(stoi(byteStr[0]), stoi(byteStr[1]));
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_18::Run_FirstPart()
{
    int corruptedLocations = 1024;
    for (int i = 0; i < corruptedLocations; ++i)
    {
        const Vector2& byte = m_IncomingBytes[i];
        m_Map[byte.y][byte.x] = '#';
    }

    Vector2 startPos{ 0, 0 };
    Vector2 endPos{ sm_mapSize - 1, sm_mapSize - 1 };
    long long shortestPath = ComputeShortestPath(startPos, endPos);

    cout << "The minimum number of steps needed to reach the exit is " << shortestPath << "." << endl;
    return EErrorCode::Success;
}

EErrorCode CChallenge_18::CleanUp_FirstPart()
{
    m_IncomingBytes.clear();
    m_Map.clear();
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_18::SetUp_SecondPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    m_Map.resize(sm_mapSize, vector<char>(sm_mapSize, '.'));

    m_IncomingBytes.reserve(lines.size());
    for (string const& line : lines)
    {
        vector<string> byteStr;
        FileHelper::SplitLine(line, ",", byteStr);
        m_IncomingBytes.emplace_back(stoi(byteStr[0]), stoi(byteStr[1]));
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_18::Run_SecondPart()
{
    int corruptedLocations = 1024;
    for (int i = 0; i < corruptedLocations; ++i)
    {
        const Vector2& byte = m_IncomingBytes[i];
        m_Map[byte.y][byte.x] = '#';
    }

    int lowerBound = corruptedLocations;
    int upperBound = (int)m_IncomingBytes.size();
    while (lowerBound < upperBound - 1)
    {
        int mid = (lowerBound + upperBound) / 2;

        // Update map
        const Vector2& byte = m_IncomingBytes[mid];
        if (m_Map[byte.y][byte.x] == '#')
        {
            // Must remove bytes
            for (int i = mid; i < upperBound; ++i)
            {
                const Vector2& byte = m_IncomingBytes[i];
                m_Map[byte.y][byte.x] = '.';
            }
        }
        else
        {
            // Must add bytes
            for (int i = lowerBound; i < mid; ++i)
            {
                const Vector2& byte = m_IncomingBytes[i];
                m_Map[byte.y][byte.x] = '#';
            }
        }

        Vector2 startPos{ 0, 0 };
        Vector2 endPos{ sm_mapSize - 1, sm_mapSize - 1 };
        long long shortestPath = ComputeShortestPath(startPos, endPos);

        if (shortestPath == LLONG_MAX)
        {
            // The first byte that will block the path is between lowerBound and mid
            upperBound = mid;
        }
        else
        {
            // The first byte that will block the path is between mid and upperBound
            lowerBound = mid;
        }
    }

    cout << "The first byte that will make the exit unreachable is at " << m_IncomingBytes[lowerBound] << "." << endl;
    return EErrorCode::Success;
}

EErrorCode CChallenge_18::CleanUp_SecondPart()
{
    m_IncomingBytes.clear();
    m_Map.clear();
    return EErrorCode::Success;
}

long long CChallenge_18::ComputeShortestPath(const Vector2& _start, const Vector2& _end)

{
    struct Node
    {
        Node() = default;
        Node(const Vector2& _pos, Direction _dir, long long _cost)
            : pos(_pos), dir(_dir), cost(_cost)
        {}

        Vector2 pos;
        Direction dir;
        long long cost = 0ll;
    };

    vector<vector<long long>> visitedCosts(m_Map.size(), vector<long long>(m_Map.size(), LLONG_MAX));

    queue<Node> nodesToVisit;
    nodesToVisit.push(Node()); // Dummy
    nodesToVisit.push({ _start, Direction::Right, 0ll });

    long long lowestScore = LLONG_MAX;
    while (true)
    {
        nodesToVisit.pop();
        if (nodesToVisit.empty())
            break;

        const Node& currentNode = nodesToVisit.front();

        if (!IsPosInMappedArea(currentNode.pos))
        {
            continue;
        }

        if (m_Map[currentNode.pos.y][currentNode.pos.x] == '#')
        {
            continue;
        }

        if (currentNode.pos == _end)
        {
            lowestScore = min(lowestScore, currentNode.cost);
            continue;
        }

        if (currentNode.cost >= lowestScore)
        {
            continue;
        }

        long long& visitedCost = visitedCosts[currentNode.pos.y][currentNode.pos.x];
        if (currentNode.cost >= visitedCost)
        {
            continue;
        }

        visitedCost = currentNode.cost;

        // Move forward
        Direction nextDir = currentNode.dir;
        Vector2 nextPos = currentNode.pos + DirectionUtils::GetDelta(currentNode.dir);
        nodesToVisit.emplace(nextPos, currentNode.dir, currentNode.cost + 1ll);

        // Turn left
        nextDir = DirectionUtils::GetPrev(currentNode.dir);
        nextPos = currentNode.pos + DirectionUtils::GetDelta(nextDir);
        nodesToVisit.emplace(nextPos, nextDir, currentNode.cost + 1ll);

        // Turn right
        nextDir = DirectionUtils::GetNext(currentNode.dir);
        nextPos = currentNode.pos + DirectionUtils::GetDelta(nextDir);
        nodesToVisit.emplace(nextPos, nextDir, currentNode.cost + 1ll);
    }

    return lowestScore;
}

bool CChallenge_18::IsPosInMappedArea(const Vector2& _pos) const
{
    return _pos.x >= 0ll && _pos.x < (long long)m_Map[0].size() &&
        _pos.y >= 0ll && _pos.y < (long long)m_Map.size();
}

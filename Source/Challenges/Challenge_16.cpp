#include <Challenges/Challenge_16.h>
#include <Helpers/FileHelpers.h>

#include <set>
#include <queue>

////////////////////////////////////////////////////////////////////////////////////////////////////
string const CChallenge_16::sm_inputFilePath = "Inputs/Input_Challenge_16.txt";



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_16::SetUp_FirstPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    m_Map.reserve(lines.size());
    const int mapWidth = (int)lines[0].size();
    for (int i = 0; i < mapWidth; ++i)
    {
        const string& line = lines[i];

        m_Map.emplace_back();
        vector<char>& mapLine = m_Map.back();
        mapLine.reserve(mapWidth);
        for (int j = 0; j < mapWidth; ++j)
        {
            if (line[j] == 'S')
            {
                m_ReindeerPos = { j, i };
                mapLine.push_back('.');
            }
            else if (line[j] == 'E')
            {
                m_EndPos = { j, i };
                mapLine.push_back('.');
            }
            else
                mapLine.push_back(line[j]);
        }
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_16::Run_FirstPart()
{
    long long lowestScore = ComputeShortestPath(m_ReindeerPos, m_EndPos);

    cout << "Lowest score: " << lowestScore << "." << endl;

    return EErrorCode::Success;
}

EErrorCode CChallenge_16::CleanUp_FirstPart()
{
    m_Map.clear();
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_16::SetUp_SecondPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    m_Map.reserve(lines.size());
    const int mapWidth = (int)lines[0].size();
    for (int i = 0; i < mapWidth; ++i)
    {
        const string& line = lines[i];

        m_Map.emplace_back();
        vector<char>& mapLine = m_Map.back();
        mapLine.reserve(mapWidth);
        for (int j = 0; j < mapWidth; ++j)
        {
            if (line[j] == 'S')
            {
                m_ReindeerPos = { j, i };
                mapLine.push_back('.');
            }
            else if (line[j] == 'E')
            {
                m_EndPos = { j, i };
                mapLine.push_back('.');
            }
            else
                mapLine.push_back(line[j]);
        }
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_16::Run_SecondPart()
{
    vector<Path> paths;
    FindShortestPaths(m_ReindeerPos, m_EndPos, paths);

    Vector2Set visitedPos;
    for (const Path& path : paths)
    {
        for (const Vector2& pos : path)
        {
            visitedPos.insert(pos);
        }
    }

    cout << "There are " << visitedPos.size() << " tiles that are part of at least one of the best paths." << endl;

#ifdef _DEBUG
    // Draw map with identified tiles
    for (int i = 0; i < m_Map.size(); ++i)
        {
            for (int j = 0; j < m_Map[0].size(); ++j)
            {
                Vector2 pos{ j, i };
                if (visitedPos.find(pos) != visitedPos.end())
                {
                    cout << 'O';
                }
                else
                    cout << m_Map[pos.y][pos.x];
            }
            cout << endl;
        }
#endif // _DEBUG

    return EErrorCode::Success;
}

EErrorCode CChallenge_16::CleanUp_SecondPart()
{
    m_Map.clear();
    return EErrorCode::Success;
}

long long CChallenge_16::ComputeShortestPath(const Vector2& _start, const Vector2& _end)
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
        nodesToVisit.emplace( nextPos, currentNode.dir, currentNode.cost + 1ll );

        // Turn left
        nextDir = DirectionUtils::GetPrev(currentNode.dir);
        nextPos = currentNode.pos + DirectionUtils::GetDelta(nextDir);
        nodesToVisit.emplace( nextPos, nextDir, currentNode.cost + 1001ll );

        // Turn right
        nextDir = DirectionUtils::GetNext(currentNode.dir);
        nextPos = currentNode.pos + DirectionUtils::GetDelta(nextDir);
        nodesToVisit.emplace( nextPos, nextDir, currentNode.cost + 1001ll );
    }

    return lowestScore;
}

void CChallenge_16::FindShortestPaths(const Vector2& _start, const Vector2& _end, vector<Path>& _paths)
{
    struct Node
    {
        Node() = default;
        Node(const Vector2& _pos, Direction _dir, long long _cost, const Path& _path)
            : pos(_pos), dir(_dir), cost(_cost), path(_path)
        {}

        Vector2 pos;
        Direction dir;
        long long cost = 0ll;
        Path path;
    };

    auto nodeCmp = [](const Node& _lhs, const Node& _rhs) 
        { 
            return (int)_lhs.dir < (int)_rhs.dir 
               || ((int)_lhs.dir == (int)_rhs.dir && _lhs.pos < _rhs.pos);
        };

    set<Node, decltype(nodeCmp)> visitedNodes(nodeCmp);

    queue<Node> nodesToVisit;
    nodesToVisit.push(Node()); // Dummy, to pop queue at beginning of loop, to allow using const ref of nodes and avoid copy
    nodesToVisit.push({ _start, Direction::Right, 0ll, Path({_start}) });

    long long lowestScore = LLONG_MAX;
    while (true)
    {
        nodesToVisit.pop();
        if (nodesToVisit.empty())
            break;

        const Node& currentNode = nodesToVisit.front();

        if (m_Map[currentNode.pos.y][currentNode.pos.x] == '#')
        {
            continue;
        }

        if (currentNode.pos == _end)
        {
            if (currentNode.cost <= lowestScore)
            {
                if (currentNode.cost < lowestScore)
                {
                    // Found a shorter path, discard previously stored ones
                    _paths.clear();
                    lowestScore = currentNode.cost;
                }

                _paths.push_back(currentNode.path);
            }
            continue;
        }

        if (currentNode.cost > lowestScore)
        {
            continue;
        }

        auto visitedNodeIt = visitedNodes.find(currentNode);
        if (visitedNodeIt != visitedNodes.end())
        {
            if (visitedNodeIt->cost < currentNode.cost)
            {
                continue;
            }
            else if (visitedNodeIt->cost > currentNode.cost)
            {
                visitedNodes.erase(visitedNodeIt);
                visitedNodes.insert(currentNode);
            }
            // else: visitedNodeIt->cost == currentNode.cost
            // We keep the visited node
            // but we still need to explore the other paths
        }
        else
        {
            visitedNodes.insert(currentNode);
        }

        // Move forward
        Direction nextDir = currentNode.dir;
        Vector2 nextPos = currentNode.pos + DirectionUtils::GetDelta(currentNode.dir);
        {
            Path newPath;
            newPath.reserve(currentNode.path.size() + 1);
            newPath.assign(currentNode.path.begin(), currentNode.path.end());
            newPath.push_back(nextPos);
            nodesToVisit.emplace( nextPos, currentNode.dir, currentNode.cost + 1ll, move(newPath) );
        }

        // Turn left
        nextDir = DirectionUtils::GetPrev(currentNode.dir);
        nextPos = currentNode.pos + DirectionUtils::GetDelta(nextDir);
        {
            Path newPath;
            newPath.reserve(currentNode.path.size() + 1);
            newPath.assign(currentNode.path.begin(), currentNode.path.end());
            newPath.push_back(nextPos);
            nodesToVisit.emplace( nextPos, nextDir, currentNode.cost + 1001ll, move(newPath) );
        }

        // Turn right
        nextDir = DirectionUtils::GetNext(currentNode.dir);
        nextPos = currentNode.pos + DirectionUtils::GetDelta(nextDir);
        {
            Path newPath;
            newPath.reserve(currentNode.path.size() + 1);
            newPath.assign(currentNode.path.begin(), currentNode.path.end());
            newPath.push_back(nextPos);
            nodesToVisit.emplace( nextPos, nextDir, currentNode.cost + 1001ll, move(newPath) );
        }
    }
}

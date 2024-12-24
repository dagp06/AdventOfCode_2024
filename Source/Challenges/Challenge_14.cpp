#include <Challenges/Challenge_14.h>
#include <Helpers/FileHelpers.h>

#include <fstream>
#include <map>
#include <set>


////////////////////////////////////////////////////////////////////////////////////////////////////
string const CChallenge_14::sm_inputFilePath = "Inputs/Input_Challenge_14.txt";
Vector2 const CChallenge_14::sm_mapSize = Vector2(101, 103);


////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_14::SetUp_FirstPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    m_Robots.reserve(lines.size());
    for (const string& line : lines)
    {
        m_Robots.emplace_back();
        Robot& robot = m_Robots.back();

        vector<string> elemStr;
        FileHelper::SplitLine(line, " ", elemStr);

        vector<string> posElemStr;
        FileHelper::SplitLine(elemStr[0], "=", posElemStr);
        vector<string> posStr;
        FileHelper::SplitLine(posElemStr[1], ",", posStr);
        robot.pos = Vector2(stoll(posStr[0]), stoll(posStr[1]));

        vector<string> velElemStr;
        FileHelper::SplitLine(elemStr[1], "=", velElemStr);
        vector<string> velStr;
        FileHelper::SplitLine(velElemStr[1], ",", velStr);
        robot.velocity = Vector2(stoll(velStr[0]), stoll(velStr[1]));
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_14::Run_FirstPart()
{
    // Move robots
    int secondsToElapse = 100;

    for (Robot& robot : m_Robots)
    {
        robot.Move(secondsToElapse);
    }

    // Compute safety factor
    int halfWidth = (int)sm_mapSize.x / 2;
    int halfHeight = (int)sm_mapSize.y / 2;
    static const int sQuadrantCount = 4;
    int robotsPerQuadrant[sQuadrantCount] { 0, 0, 0, 0 };
    for (const Robot& robot : m_Robots)
    {
        if (robot.pos.x < halfWidth)
        {
            if (robot.pos.y < halfHeight)
                ++robotsPerQuadrant[1];
            else if (robot.pos.y > halfHeight)
                ++robotsPerQuadrant[2];
        }
        else if (robot.pos.x > halfWidth)
        {
            if (robot.pos.y < halfHeight)
                ++robotsPerQuadrant[0];
            else if (robot.pos.y > halfHeight)
                ++robotsPerQuadrant[3];
        }
    }

    int safetyFactor = 1;
    for (int i = 0; i < sQuadrantCount; ++i)
    {
        safetyFactor *= robotsPerQuadrant[i];
    }

    cout << "The safety factor after " << secondsToElapse << " seconds will be " << safetyFactor << endl;
    return EErrorCode::Success;
}

EErrorCode CChallenge_14::CleanUp_FirstPart()
{
    m_Robots.clear();
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_14::SetUp_SecondPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    m_Robots.reserve(lines.size());
    for (const string& line : lines)
    {
        m_Robots.emplace_back();
        Robot& robot = m_Robots.back();

        vector<string> elemStr;
        FileHelper::SplitLine(line, " ", elemStr);

        vector<string> posElemStr;
        FileHelper::SplitLine(elemStr[0], "=", posElemStr);
        vector<string> posStr;
        FileHelper::SplitLine(posElemStr[1], ",", posStr);
        robot.pos = Vector2(stoll(posStr[0]), stoll(posStr[1]));

        vector<string> velElemStr;
        FileHelper::SplitLine(elemStr[1], "=", velElemStr);
        vector<string> velStr;
        FileHelper::SplitLine(velElemStr[1], ",", velStr);
        robot.velocity = Vector2(stoll(velStr[0]), stoll(velStr[1]));
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_14::Run_SecondPart()
{
    int secondsElapsed = 0;

    m_RobotsMap.reserve(sm_mapSize.y);
    for (int i = 0; i < sm_mapSize.y; ++i)
    {
        m_RobotsMap.emplace_back();
        vector<int>& row = m_RobotsMap.back();
        row.resize(sm_mapSize.x);
        fill(row.begin(), row.end(), 0);
    }

    for (Robot& robot : m_Robots)
    {
        ++m_RobotsMap[robot.pos.y][robot.pos.x];
    }

    while (true)
    {
        ++secondsElapsed;
        for (Robot& robot : m_Robots)
        {
            --m_RobotsMap[robot.pos.y][robot.pos.x];
            robot.Move(1);
            ++m_RobotsMap[robot.pos.y][robot.pos.x];
        }

        float entropy = ComputeEntropy();
        if (entropy < 5.f)
            break;
    }

    cout << "The robots will display their Easter egg after " << secondsElapsed << " seconds." << endl;

#ifdef _DEBUG
    DrawMap();
#endif

    return EErrorCode::Success;
}

EErrorCode CChallenge_14::CleanUp_SecondPart()
{
    m_Robots.clear();
    m_RobotsMap.clear();
    return EErrorCode::Success;
}


void CChallenge_14::DrawMap() const
{
    int halfWidth = (int)sm_mapSize.x / 2;
    int halfHeight = (int)sm_mapSize.y / 2;
    for (int i = 0; i < sm_mapSize.y; ++i)
    {
        if (i == halfHeight)
        {
            cout << string(sm_mapSize.x, '-');
        }
        else
        {
            for (int j = 0; j < sm_mapSize.x; ++j)
            {
                if (j == halfWidth)
                {
                    cout << '|';
                }
                else
                {
                    cout << (m_RobotsMap[i][j] > 0 ? to_string(m_RobotsMap[i][j]) : ".");
                }
            }
        }
        cout << endl;
    }
}

float CChallenge_14::ComputeEntropy() const
{
    int cellSize = 12;
    map<Vector2, int> grid;
    set<Vector2> processedPoints;

    for (const Robot& robot : m_Robots)
    {
        if (processedPoints.find(robot.pos) != processedPoints.end())
        {
            continue;
        }
        processedPoints.insert(robot.pos);

        int cellX = (int)robot.pos.x / cellSize;
        int cellY = (int)robot.pos.y / cellSize;
        ++grid[{cellX, cellY}];
    }

    int totalPoints = (int)processedPoints.size();

    float entropy = 0.f;
    for (const pair<Vector2, int>& cell : grid)
    {
        float p = (float)cell.second / totalPoints;
        if (p > 0.f)
        {
            entropy -= p * log2(p);
        }
    }

    return entropy;
}

void CChallenge_14::Robot::Move(int _secondsToElapse)
{
    pos += velocity * _secondsToElapse;

    pos.x %= CChallenge_14::sm_mapSize.x;
    if (pos.x < 0)
        pos.x = CChallenge_14::sm_mapSize.x + pos.x;

    pos.y %= CChallenge_14::sm_mapSize.y;
    if (pos.y < 0)
        pos.y = CChallenge_14::sm_mapSize.y + pos.y;
}

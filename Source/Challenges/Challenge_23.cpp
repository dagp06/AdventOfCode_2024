#include <Challenges/Challenge_23.h>
#include <Helpers/FileHelpers.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
string const CChallenge_23::sm_inputFilePath = "Inputs/Input_Challenge_23.txt";

void CChallenge_23::ExtractAllClusters(const string& clusterStart, const set<string>& currentCluster, const set<string>& computersToVisit, set<set<string>>& visitedClusters, map<int, set<set<string>>>& outClusters, const int clusterSize /*= INT32_MAX*/)
{
    for (const string& nextComputer : computersToVisit)
    {
        set<string> newCluster = currentCluster;
        newCluster.insert(nextComputer);

        if (visitedClusters.find(newCluster) != visitedClusters.end())
            continue;

        visitedClusters.insert(newCluster);

        const set<string>& nextComputerConnections = m_Connections[nextComputer];

        if (clusterSize != INT32_MAX)
        {
            if (newCluster.size() == clusterSize)
            {
                // New cluster found
                if (nextComputerConnections.find(clusterStart) != nextComputerConnections.end())
                    outClusters[(int)newCluster.size()].insert(newCluster);

                continue;
            }
        }
        else
        {
            // New cluster found
            if (nextComputerConnections.find(clusterStart) != nextComputerConnections.end())
                outClusters[(int)newCluster.size()].insert(newCluster);
        }

        // Next computers to visit is the intersection between previous computers to visit and next computer's connections
        set<string> nextComputersToVisit;
        for (const string& computerToVisit : computersToVisit)
        {
            if (nextComputerConnections.find(computerToVisit) != nextComputerConnections.end())
                nextComputersToVisit.insert(computerToVisit);
        }

        if (nextComputersToVisit.empty())
            continue;

        ExtractAllClusters(clusterStart, newCluster, nextComputersToVisit, visitedClusters, outClusters, clusterSize);
    }
}

void CChallenge_23::PrintClusters(const map<int, set<set<string>>>& clusters) const
{
    for (map<int, set<set<string>>>::const_iterator it = clusters.begin(); it != clusters.end(); ++it)
    {
        for (set<set<string>>::const_iterator clusterIt = it->second.begin(); clusterIt != it->second.end(); ++clusterIt)
        {
            string separator = "";
            for (const string& computerName : *clusterIt)
            {
                cout << separator << computerName;
                separator = ",";
            }
            cout << endl;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_23::SetUp_FirstPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    for (const string& line : lines)
    {
        vector<string> computerNames;
        FileHelper::SplitLine(line, "-", computerNames);
        m_Connections[computerNames[0]].insert(computerNames[1]);
        m_Connections[computerNames[1]].insert(computerNames[0]);
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_23::Run_FirstPart()
{
    map<int, set<set<string>>> allComputerClusters;
    for (const pair<string, set<string>>& computerConnections : m_Connections)
    {
        if (computerConnections.first[0] != 't')
            continue;

        set<string> currentCluster;
        currentCluster.insert(computerConnections.first);
        set<set<string>> visitedClusters;
        ExtractAllClusters(computerConnections.first, currentCluster, computerConnections.second, visitedClusters, allComputerClusters, 3);
    }

    int clusterCount = 0;
    for (map<int, set<set<string>>>::const_iterator it = allComputerClusters.begin(); it != allComputerClusters.end(); ++it)
    {
        clusterCount += (int)it->second.size();

#ifdef _DEBUG
        PrintClusters(allComputerClusters);
#endif // _DEBUG
    }

#ifndef BENCHMARK
    cout << "There are " << clusterCount << " sets of three inter-connected computers with at least one computer with a name that starts with t." << endl;
#endif // !BENCHMARK

    return EErrorCode::Success;
}

EErrorCode CChallenge_23::CleanUp_FirstPart()
{
    m_Connections.clear();
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_23::SetUp_SecondPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    for (const string& line : lines)
    {
        vector<string> computerNames;
        FileHelper::SplitLine(line, "-", computerNames);
        m_Connections[computerNames[0]].insert(computerNames[1]);
        m_Connections[computerNames[1]].insert(computerNames[0]);
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_23::Run_SecondPart()
{
    set<set<string>> visitedClusters;
    map<int, set<set<string>>> allComputerClusters;
    for (const pair<string, set<string>>& computerConnections : m_Connections)
    {
        set<string> currentCluster;
        currentCluster.insert(computerConnections.first);
        ExtractAllClusters(computerConnections.first, currentCluster, computerConnections.second, visitedClusters, allComputerClusters);
    }

#ifdef _DEBUG
    PrintClusters(allComputerClusters);
#endif // _DEBUG

    string password;
    const set<set<string>>& largestClusters = prev(allComputerClusters.end())->second;
    if (largestClusters.size() == 1)
    {
        const set<string>& largestCluster = *largestClusters.begin();

        password.reserve(largestCluster.size() * 3 - 1);
        string separator = "";
        for (const string& computerName : largestCluster)
        {
            password.append(separator);
            password.append(computerName);
            separator = ",";
        }

#ifndef BENCHMARK
        cout << "The password is " << password << "." << endl;
#endif // !BENCHMARK
    }
    else
    {
        cout << "Error - Couldn't find the biggest inter-connected cluster." << endl;
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_23::CleanUp_SecondPart()
{
    m_Connections.clear();
    return EErrorCode::Success;
}

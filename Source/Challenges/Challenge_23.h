#pragma once

#include <Challenges/Common/Challenge.h>

#include <map>
#include <set>

using namespace std;

class CChallenge_23 : public CChallenge
{
private:
    virtual EErrorCode SetUp_FirstPart() override;
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override;

    virtual EErrorCode SetUp_SecondPart() override;
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override;

    static string const sm_inputFilePath;

    map<string, set<string>> m_Connections;
    void ExtractAllClusters(const string& clusterStart, const set<string>& currentCluster, const set<string>& computersToVisit, set<set<string>>& visitedClusters, map<int, set<set<string>>>& outClusters, const int clusterSize = INT32_MAX);
    void PrintClusters(const map<int, set<set<string>>>& clusters) const;
};

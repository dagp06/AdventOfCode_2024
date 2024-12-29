#pragma once

#include <Challenges/Common/Challenge.h>

#include <map>
#include <set>

using namespace std;

class CChallenge_19 : public CChallenge
{
private:
    typedef map<string, long long> DesignArrangementsMap;

    virtual EErrorCode SetUp_FirstPart() override;
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override;

    virtual EErrorCode SetUp_SecondPart() override;
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override;

    bool IsDesignPossible(const string& _design, set<string>& _possibleDesigns, set<string>& _impossibleDesigns) const;
    long long CountPossibleArrangementsForDesign(const string& _design, DesignArrangementsMap& _possibleDesignArrangements, set<string>& _impossibleDesigns) const;
    bool DoesDesignStartWith(const string& _design, const string& _start) const;

    static string const sm_inputFilePath;

    vector<string> m_Towels;
    vector<string> m_Designs;
};

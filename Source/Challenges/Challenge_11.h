#pragma once

#include <Challenges/Common/Challenge.h>

#include <map>

using namespace std;

class CChallenge_11 : public CChallenge
{
private:
    
    struct Stone
    {
        Stone(long long _number) : m_Number(_number) {}

        long long m_Number = 0;

        Stone* m_NextStone = nullptr;
    };

    virtual EErrorCode SetUp_FirstPart() override;
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override;

    virtual EErrorCode SetUp_SecondPart() override;
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override;

    void Blink_Part1();
    void Blink_Part2();
    int CountStones_Part1() const;
    long long CountStones_Part2() const;

#ifdef _DEBUG
    void PrintStoneArrangement_Part1() const;
    void PrintStoneArrangement_Part2() const;
#endif

    static string const sm_inputFilePath;

    Stone* m_FirstStone;

    typedef map<long long, long long> StoneOccurrencesMap;
    StoneOccurrencesMap m_StoneOccurrences;

};

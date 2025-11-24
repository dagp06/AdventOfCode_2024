#pragma once

#include <Challenges/Common/Challenge.h>

#include <map>

using namespace std;

class CChallenge_22 : public CChallenge
{
private:
    virtual EErrorCode SetUp_FirstPart() override;
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override;

    virtual EErrorCode SetUp_SecondPart() override;
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override;

    static string const sm_inputFilePath;

    vector<long long> m_SecretNumbers;
    long long ComputNextSecretNumber(const long long secretNumber, const int count = 1);

    map<vector<int>, int> m_SequenceTotals;
    void UpdateSequenceWithVariation(vector<int>& currentSequence, int variation);
};

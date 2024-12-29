#pragma once

#include <Challenges/Common/Challenge.h>

using namespace std;

class CChallenge_17 : public CChallenge
{
private:
    virtual EErrorCode SetUp_FirstPart() override;
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override;

    virtual EErrorCode SetUp_SecondPart() override;
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override;

    long long GetComboOperand(const uint8_t& _literalOperand) const;
    void RunProgram(vector<uint8_t>& _outValues);
    bool ComparePrograms(const vector<uint8_t>& _programA, const vector<uint8_t>& _programB) const;

    static string const sm_inputFilePath;

    long long m_RegisterA = 0;
    long long m_RegisterB = 0;
    long long m_RegisterC = 0;

    vector<uint8_t> m_Program;
};

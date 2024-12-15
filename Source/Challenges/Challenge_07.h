#pragma once

#include "Common/Challenge.h"

using namespace std;

class CChallenge_07 : public CChallenge
{
private:
    struct CalibrationEquation
    {
        long long m_Answer = 0;
        vector<int> m_Operands;
    };

    enum class Operation
    {
        Addition,
        Multiplication,
        Concatenation,
        Count,
        CountPart1 = Concatenation,
        CountPart2 = Count
    };

    virtual EErrorCode SetUp_FirstPart() override;
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override;

    virtual EErrorCode SetUp_SecondPart() override;
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override;

    bool EvaluateEquation(const CalibrationEquation& _equation, Operation _opCount) const;
    bool EvaluateEquation(const CalibrationEquation& _equation, long long _currentResult, int _nextIndex, Operation _operation, Operation _opCount) const;

    static string const sm_inputFilePath;

    vector<CalibrationEquation> m_CalibrationEquations;
};

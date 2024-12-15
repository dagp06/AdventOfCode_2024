#include "Challenge_07.h"
#include "../Helpers/FileHelpers.h"



////////////////////////////////////////////////////////////////////////////////////////////////////
string const CChallenge_07::sm_inputFilePath = "Inputs/Input_Challenge_07.txt";



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_07::SetUp_FirstPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    for (const string& line : lines)
    {
        m_CalibrationEquations.push_back(CalibrationEquation());
        CalibrationEquation& equation = m_CalibrationEquations.back();

        vector<string> lineSplit;
        FileHelper::SplitLine(line, ":", lineSplit);
        equation.m_Answer = stoll(lineSplit[0]);

        vector<string> operandsStr;
        FileHelper::SplitLine(lineSplit[1], " ", operandsStr);
        equation.m_Operands.reserve(operandsStr.size());
        for (int i = 1; i < operandsStr.size(); ++i) // Skip first elem, empty string
        {
            equation.m_Operands.push_back(stoi(operandsStr[i]));
        }
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_07::Run_FirstPart()
{
    long long totalCalibrationResult = 0ll;

    for (const CalibrationEquation& equation : m_CalibrationEquations)
    {
        if (EvaluateEquation(equation, Operation::CountPart1))
        {
            totalCalibrationResult += equation.m_Answer;
        }
    }

    cout << "The total calibration result is " << totalCalibrationResult << "." << endl;
    return EErrorCode::Success;
}

EErrorCode CChallenge_07::CleanUp_FirstPart()
{
    m_CalibrationEquations.clear();
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_07::SetUp_SecondPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    for (const string& line : lines)
    {
        m_CalibrationEquations.push_back(CalibrationEquation());
        CalibrationEquation& equation = m_CalibrationEquations.back();

        vector<string> lineSplit;
        FileHelper::SplitLine(line, ":", lineSplit);
        equation.m_Answer = stoll(lineSplit[0]);

        vector<string> operandsStr;
        FileHelper::SplitLine(lineSplit[1], " ", operandsStr);
        equation.m_Operands.reserve(operandsStr.size());
        for (int i = 1; i < operandsStr.size(); ++i) // Skip first elem, empty string
        {
            equation.m_Operands.push_back(stoi(operandsStr[i]));
        }
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_07::Run_SecondPart()
{
    long long totalCalibrationResult = 0ll;

    for (const CalibrationEquation& equation : m_CalibrationEquations)
    {
        if (EvaluateEquation(equation, Operation::CountPart2))
        {
            totalCalibrationResult += equation.m_Answer;
        }
    }

    cout << "The total calibration result is " << totalCalibrationResult << "." << endl;
    return EErrorCode::Success;
}

EErrorCode CChallenge_07::CleanUp_SecondPart()
{
    m_CalibrationEquations.clear();
    return EErrorCode::Success;
}


bool CChallenge_07::EvaluateEquation(const CalibrationEquation& _equation, Operation _opCount) const
{
    for (int op = 0; op < (int)_opCount; ++op)
    {
        if (EvaluateEquation(_equation, _equation.m_Operands[0], 1, (Operation)op, _opCount))
        {
            return true;
        }
    }

    return false;
}

bool CChallenge_07::EvaluateEquation(const CalibrationEquation& _equation, long long _currentResult, int _nextIndex, Operation _operation, Operation _opCount) const
{
    switch (_operation)
    {
    case CChallenge_07::Operation::Addition:
        _currentResult += _equation.m_Operands[_nextIndex];
        break;
    case CChallenge_07::Operation::Multiplication:
        _currentResult *= _equation.m_Operands[_nextIndex];
        break;
    case CChallenge_07::Operation::Concatenation:
        _currentResult = stoll(to_string(_currentResult) + to_string(_equation.m_Operands[_nextIndex]));
        break;
    default:
        break;
    }

    if (_nextIndex == _equation.m_Operands.size() - 1)
    {
        // We're done, validate the equation
        return _currentResult == _equation.m_Answer;
    }

    for (int op = 0; op < (int)_opCount; ++op)
    {
        if (EvaluateEquation(_equation, _currentResult, _nextIndex + 1, (Operation)op, _opCount))
        {
            return true;
        }
    }

    return false;
}
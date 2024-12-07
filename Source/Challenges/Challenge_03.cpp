#include "Challenge_03.h"
#include "../Helpers/FileHelpers.h"

#include <regex>

////////////////////////////////////////////////////////////////////////////////////////////////////
string const CChallenge_03::sm_inputFilePath = "Inputs/Input_Challenge_03.txt";



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_03::SetUp_FirstPart()
{
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, m_Lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_03::Run_FirstPart()
{
    long long totalSum = 0ll;
    for (const string& line : m_Lines)
    {
        for (int i = 0; i < line.size(); )
        {
            ProcessMul(line, i, totalSum);
        }
    }

    cout << "The total sum of multiplication results is " << totalSum << "." << endl;

    return EErrorCode::Success;
}

EErrorCode CChallenge_03::CleanUp_FirstPart()
{
    m_Lines.clear();
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_03::SetUp_SecondPart()
{
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, m_Lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }
    
    return EErrorCode::Success;
}

EErrorCode CChallenge_03::Run_SecondPart()
{
    long long totalSum = 0ll;
    bool mulEnabled = true;
    for (const string& line : m_Lines)
    {
        for (int i = 0; i < line.size(); )
        {
            if (mulEnabled)
            {
                if (ProcessMul(line, i, totalSum))
                    continue;

                if (ProcessPattern(line, --i, "don't")) // i incremented in ProcessMul, need to reevaluate last character
                {
                    mulEnabled = false;
                    continue;
                }
            }
            else
            {
                if (ProcessPattern(line, i, "do()"))
                {
                    mulEnabled = true;
                    continue;
                }
            }
        }
    }

    cout << "The total sum of multiplication results is " << totalSum << "." << endl;

    return EErrorCode::Success;
}

EErrorCode CChallenge_03::CleanUp_SecondPart()
{
    m_Lines.clear();
    return EErrorCode::Success;
}

bool CChallenge_03::ProcessMul(const string& line, int& i, long long& totalSum) const
{
    if (!ProcessPattern(line, i, "mul("))
        return false;

    // READ X
    string sX;
    sX.reserve(3);
    bool isNumberValid = true;
    for (int j = 0; j < 3; ++j)
    {
        const char& character = line[i + j];
        if (isdigit(character))
        {
            sX.push_back(character);
        }
        else if (character == ',' && j > 0)
        {
            break;
        }
        else
        {
            isNumberValid = false;
            i += j;
            break;
        }
    }

    if (!isNumberValid)
    {
        ++i;
        return false;
    }

    i += static_cast<int>(sX.size());

    if (!ProcessPattern(line, i, ","))
        return false;

    // READ Y
    string sY;
    sY.reserve(3);
    for (int j = 0; j < 3; ++j)
    {
        const char& character = line[i + j];
        if (isdigit(character))
        {
            sY.push_back(character);
        }
        else if (character == ')' && j > 0)
        {
            break;
        }
        else
        {
            isNumberValid = false;
            i += j;
            break;
        }
    }

    if (!isNumberValid)
    {
        ++i;
        return false;
    }

    i += static_cast<int>(sY.size());

    if (!ProcessPattern(line, i, ")"))
        return false;

    int x = stoi(sX);
    int y = stoi(sY);
    totalSum += static_cast<long long>(x * y);

    return true;
}

bool CChallenge_03::ProcessPattern(const string& line, int& i, const string& pattern) const
{
    for (const char& ch : pattern)
    {
        if (line[i++] != ch)
        {
            return false;
        }
    }

    return true;
}

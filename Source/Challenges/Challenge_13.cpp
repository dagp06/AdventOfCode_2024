#include <Challenges/Challenge_13.h>
#include <Helpers/FileHelpers.h>


////////////////////////////////////////////////////////////////////////////////////////////////////
string const CChallenge_13::sm_inputFilePath = "Inputs/Input_Challenge_13.txt";
int const CChallenge_13::sm_buttonACost = 3;
int const CChallenge_13::sm_buttonBCost = 1;



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_13::SetUp_FirstPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    for (int i = 0; i < lines.size(); i += 4)
    {
        m_ClawMachines.emplace_back();
        ClawMachine& clawMachine = m_ClawMachines.back();
        // Button A
        vector<string> tempStr;
        FileHelper::SplitLine(lines[i], "Button A: X+", tempStr);
        vector<string> numStr;
        FileHelper::SplitLine(tempStr[1], ", Y+", numStr);
        clawMachine.m_DeltaA = Vector2(stoi(numStr[0]), stoi(numStr[1]));

        // Button B
        tempStr.clear();
        FileHelper::SplitLine(lines[i + 1], "Button B: X+", tempStr);
        numStr.clear();
        FileHelper::SplitLine(tempStr[1], ", Y+", numStr);
        clawMachine.m_DeltaB = Vector2(stoi(numStr[0]), stoi(numStr[1]));

        // Prize
        tempStr.clear();
        FileHelper::SplitLine(lines[i + 2], "Prize: X=", tempStr);
        numStr.clear();
        FileHelper::SplitLine(tempStr[1], ", Y=", numStr);
        clawMachine.m_PrizePos = Vector2(stoi(numStr[0]), stoi(numStr[1]));
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_13::Run_FirstPart()
{
    unsigned long long totalTokenSpent = 0ll;

    for (int i = 0; i < m_ClawMachines.size(); ++i)
    {
        unsigned long long tokenSpent = ComputeTokenSpent(m_ClawMachines[i]);
        totalTokenSpent += tokenSpent;

#ifdef _DEBUG
        cout << i << " - " << (tokenSpent ? to_string(tokenSpent) : "NONE") << endl;
#endif
    }

    cout << "The fewest tokens you'd have to spend to win all possible prizes is " << totalTokenSpent << "." << endl;

    return EErrorCode::Success;
}

EErrorCode CChallenge_13::CleanUp_FirstPart()
{
    m_ClawMachines.clear();
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_13::SetUp_SecondPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    for (int i = 0; i < lines.size(); i += 4)
    {
        m_ClawMachines.emplace_back();
        ClawMachine& clawMachine = m_ClawMachines.back();
        // Button A
        vector<string> tempStr;
        FileHelper::SplitLine(lines[i], "Button A: X+", tempStr);
        vector<string> numStr;
        FileHelper::SplitLine(tempStr[1], ", Y+", numStr);
        clawMachine.m_DeltaA = Vector2(stoi(numStr[0]), stoi(numStr[1]));

        // Button B
        tempStr.clear();
        FileHelper::SplitLine(lines[i + 1], "Button B: X+", tempStr);
        numStr.clear();
        FileHelper::SplitLine(tempStr[1], ", Y+", numStr);
        clawMachine.m_DeltaB = Vector2(stoi(numStr[0]), stoi(numStr[1]));

        // Prize
        tempStr.clear();
        FileHelper::SplitLine(lines[i + 2], "Prize: X=", tempStr);
        numStr.clear();
        FileHelper::SplitLine(tempStr[1], ", Y=", numStr);
        clawMachine.m_PrizePos = Vector2(10000000000000 + stoi(numStr[0]), 10000000000000 + stoi(numStr[1]));
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_13::Run_SecondPart()
{
    unsigned long long totalTokenSpent = 0ll;

    for (int i = 0; i < m_ClawMachines.size(); ++i)
    {
        unsigned long long tokenSpent = ComputeTokenSpent(m_ClawMachines[i]);
        totalTokenSpent += tokenSpent;
        
#ifdef _DEBUG
        cout << i << " - " << (tokenSpent ? to_string(tokenSpent) : "NONE") << endl;
#endif
    }

    cout << "The fewest tokens you'd have to spend to win all possible prizes is " << totalTokenSpent << "." << endl;

    return EErrorCode::Success;
}

EErrorCode CChallenge_13::CleanUp_SecondPart()
{
    m_ClawMachines.clear();
    return EErrorCode::Success;
}

unsigned long long CChallenge_13::ComputeTokenSpent(const ClawMachine& _clawMachine)
{
    unsigned long long buttonAPressed = 0ll;
    unsigned long long buttonBPressed = 0ll;

    if (_clawMachine.Solve(buttonAPressed, buttonBPressed))
    {
        return buttonAPressed * sm_buttonACost + buttonBPressed * sm_buttonBCost;
    }

    return 0ll;
}

bool CChallenge_13::ClawMachine::Solve(unsigned long long& buttonAPressed, unsigned long long& buttonBPressed) const
{
    double a = (double)(m_PrizePos.x * m_DeltaB.y - m_PrizePos.y * m_DeltaB.x) / (m_DeltaA.x * m_DeltaB.y - m_DeltaA.y * m_DeltaB.x);
    double b = (double)(m_PrizePos.x * m_DeltaA.y - m_PrizePos.y * m_DeltaA.x) / (m_DeltaA.y * m_DeltaB.x - m_DeltaA.x * m_DeltaB.y);

    if (a >= 0.0 && (a - (unsigned long long)a) < 0.0001 && b >= 0.0 && (b - (unsigned long long)b) <  0.0001)
    {
        buttonAPressed = (unsigned long long)a;
        buttonBPressed = (unsigned long long)b;
        return true;
    }

    return false;
}

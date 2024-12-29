#include <Challenges/Challenge_17.h>
#include <Helpers/FileHelpers.h>

#include <bitset>

////////////////////////////////////////////////////////////////////////////////////////////////////
string const CChallenge_17::sm_inputFilePath = "Inputs/Input_Challenge_17.txt";




////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_17::SetUp_FirstPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    vector<string> regAStr;
    FileHelper::SplitLine(lines[0], ": ", regAStr);
    m_RegisterA = stoll(regAStr[1]);

    vector<string> regBStr;
    FileHelper::SplitLine(lines[1], ": ", regBStr);
    m_RegisterB = stoll(regBStr[1]);

    vector<string> regCStr;
    FileHelper::SplitLine(lines[2], ": ", regCStr);
    m_RegisterC = stoll(regCStr[1]);

    vector<string> programStr;
    FileHelper::SplitLine(lines[4], ": ", programStr);
    vector<string> programOpsStr;
    FileHelper::SplitLine(programStr[1], ",", programOpsStr);
    m_Program.reserve(programOpsStr.size());
    for (const string& opStr : programOpsStr)
    {
        m_Program.push_back((uint8_t)(opStr[0] - '0'));
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_17::Run_FirstPart()
{
    vector<uint8_t> outValues;
    int instructionPointer = 0;

    RunProgram(outValues);

    cout << "After the program halts, its output is: ";
    string separator = "";
    for (int i = 0; i < outValues.size(); ++i)
    {
        cout << separator << (unsigned)outValues[i];
        separator = ",";
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_17::CleanUp_FirstPart()
{
    m_Program.clear();
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_17::SetUp_SecondPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    vector<string> programStr;
    FileHelper::SplitLine(lines[4], ": ", programStr);
    vector<string> programOpsStr;
    FileHelper::SplitLine(programStr[1], ",", programOpsStr);
    m_Program.reserve(programOpsStr.size());
    for (const string& opStr : programOpsStr)
    {
        m_Program.push_back((uint8_t)(opStr[0] - '0'));
    }

#ifdef _DEBUG
    cout << lines[4] << endl;
#endif // _DEBUG

    return EErrorCode::Success;
}

EErrorCode CChallenge_17::Run_SecondPart()
{
    long long initialRegAValue = 0;
    for (int i = (int)m_Program.size() - 1; i >= 0; --i)
    {
        initialRegAValue = (initialRegAValue << 3) - 1;

        vector<uint8_t> truncatedProgram = m_Program;
        truncatedProgram.erase(truncatedProgram.begin(), truncatedProgram.begin() + i);

        vector<uint8_t> outValues;
        outValues.reserve(truncatedProgram.size());
#ifdef _DEBUG
        int j = -1;
#endif // _DEBUG
        while (!ComparePrograms(outValues, truncatedProgram))
        {
            outValues.clear();

#ifdef _DEBUG
            ++j;
#endif // _DEBUG

            // Reinitialize registers
            m_RegisterA = ++initialRegAValue;
            m_RegisterB = 0ll;
            m_RegisterC = 0ll;

            RunProgram(outValues);
        }

#ifdef _DEBUG
        cout << "Iteration " << m_Program.size() - i << " - (" << j << " tests) - New A = " << bitset<64>(initialRegAValue) << " (" << (initialRegAValue) << ")" << endl;
#endif // _DEBUG

    }

    cout << "The lowest value for register A that causes the program to output itself is " << initialRegAValue;

    return EErrorCode::Success;
}

EErrorCode CChallenge_17::CleanUp_SecondPart()
{
    m_Program.clear();
    return EErrorCode::Success;
}

long long CChallenge_17::GetComboOperand(const uint8_t& _literalOperand) const
{
    switch (_literalOperand)
    {
    case 0:
    case 1:
    case 2:
    case 3:
        return _literalOperand;
    case 4:
        return m_RegisterA;
    case 5:
        return m_RegisterB;
    case 6:
        return m_RegisterC;
    default:
        return 0;
    }
}

void CChallenge_17::RunProgram(vector<uint8_t>& _outValues)
{
    int instructionPointer = 0;
    while (instructionPointer < m_Program.size())
    {
        const uint8_t& opCode = m_Program[instructionPointer];
        const uint8_t& literalOperand = m_Program[instructionPointer + 1];

        switch (opCode)
        {
            case 0:
            {
                m_RegisterA >>= GetComboOperand(literalOperand);
                break;
            }
            case 1:
            {
                m_RegisterB ^= literalOperand;

                break;
            }
            case 2:
            {
                m_RegisterB = GetComboOperand(literalOperand) % 8;
                break;
            }
            case 3:
            {
                if (m_RegisterA != 0)
                {
                    instructionPointer = literalOperand;
                    continue;
                }
                break;
            }
            case 4:
            {
                m_RegisterB ^= m_RegisterC;
                break;
            }
            case 5:
            {
                _outValues.push_back(GetComboOperand(literalOperand) % 8);
                break;
            }
            case 6:
            {
                m_RegisterB = (m_RegisterA >> GetComboOperand(literalOperand));
                break;
            }
            case 7:
            {
                m_RegisterC = (m_RegisterA >> GetComboOperand(literalOperand));
                break;
            }
        }

        instructionPointer += 2;
    }
}

bool CChallenge_17::ComparePrograms(const vector<uint8_t>& _programA, const vector<uint8_t>& _programB) const
{
    if (_programA.size() != _programB.size())
    {
        return false;
    }

    for (int i = 0; i < _programA.size(); ++i)
    {
        if (_programA[i] != _programB[i])
        {
            return false;
        }
    }

    return true;
}

#include "Challenge_11.h"
#include "../Helpers/FileHelpers.h"




////////////////////////////////////////////////////////////////////////////////////////////////////
string const CChallenge_11::sm_inputFilePath = "Inputs/Input_Challenge_11.txt";




////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_11::SetUp_FirstPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    vector<string> numStrings;
    FileHelper::SplitLine(lines[0], " ", numStrings);

    m_FirstStone = new Stone(stoi(numStrings[0]));
    Stone* currentStone = m_FirstStone;
    for (int i = 1; i < numStrings.size(); ++i)
    {
        currentStone->m_NextStone = new Stone(stoi(numStrings[i]));
        currentStone = currentStone->m_NextStone;
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_11::Run_FirstPart()
{
    int blinkCount = 25;

#ifdef _DEBUG
    cout << "Initial arrangement:" << endl;
    PrintStoneArrangement_Part1();
    cout << endl;
#endif

    for (int i = 0; i < blinkCount; ++i)
    {
        Blink_Part1();

#ifdef _DEBUG
        cout << "After " << i + 1 << (i == 0 ? " blink:" : " blinks:") << endl;
        PrintStoneArrangement_Part1();
        cout << endl;
#endif
    }

    int stoneCount = CountStones_Part1();
#ifndef BENCHMARK
    cout << "After blinking " << blinkCount << (blinkCount == 1 ? " time" : " times") << ", there would be " << stoneCount << " stones." << endl;
#endif
    return EErrorCode::Success;
}

EErrorCode CChallenge_11::CleanUp_FirstPart()
{
    const Stone* currentStone = m_FirstStone;
    while (currentStone != nullptr)
    {
        const Stone* nextStone = currentStone->m_NextStone;
        delete currentStone;

        currentStone = nextStone;
    }

    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_11::SetUp_SecondPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    vector<string> numStrings;
    FileHelper::SplitLine(lines[0], " ", numStrings);

    for (int i = 0; i < numStrings.size(); ++i)
    {
        ++m_StoneOccurrences[stoll(numStrings[i])];
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_11::Run_SecondPart()
{
    int blinkCount = 75;

    for (int i = 0; i < blinkCount; ++i)
    {
#ifndef BENCHMARK
        cout << i + 1 << endl;
#endif

        Blink_Part2();

#ifdef _DEBUG
        cout << "After " << i + 1 << (i == 0 ? " blink: " : " blinks: ") << CountStones_Part2() << " stones" << endl;
        PrintStoneArrangement_Part2();
        cout << endl;
#endif
    }

    long long stoneCount = CountStones_Part2();

#ifndef BENCHMARK
    cout << "After blinking " << blinkCount << (blinkCount == 1 ? " time" : " times") << ", there would be " << stoneCount << " stones." << endl;
#endif

    return EErrorCode::Success;
}

EErrorCode CChallenge_11::CleanUp_SecondPart()
{
    m_StoneOccurrences.clear();

    return EErrorCode::Success;
}


void CChallenge_11::Blink_Part1()
{
    Stone* currentStone = m_FirstStone;
    while (currentStone != nullptr)
    {
        if (currentStone->m_Number == 0ll)
        {
            // Rule #1
            currentStone->m_Number = 1ll;

            currentStone = currentStone->m_NextStone;
            continue;
        }

        string numStr = to_string(currentStone->m_Number);
        if (numStr.size() % 2 == 0)
        {
            // Rule #2
            int halfNumLength = (int)numStr.size() / 2;
            currentStone->m_Number = stoll(numStr.substr(0, halfNumLength));
            Stone* newStone = new Stone(stoll(numStr.substr(halfNumLength, halfNumLength)));

            newStone->m_NextStone = currentStone->m_NextStone;
            currentStone->m_NextStone = newStone;

            currentStone = newStone->m_NextStone;
            continue;
        }

        // Rule #3
        currentStone->m_Number *= 2024ll;

        currentStone = currentStone->m_NextStone;
    }
}


void CChallenge_11::Blink_Part2()
{
    StoneOccurrencesMap newMap;

    for (const pair<long long, long long>& pair : m_StoneOccurrences)
    {
        const long long& stoneNumber = pair.first;
        const long long& stoneOccurrences = pair.second;

        if (stoneOccurrences == 0)
            continue;

        if (stoneNumber == 0ll)
        {
            // Rule #1
            newMap[1] = stoneOccurrences;
            continue;
        }

        string numStr = to_string(stoneNumber);
        if (numStr.size() % 2 == 0)
        {
            // Rule #2
            int halfNumLength = (int)numStr.size() / 2;
            newMap[stoll(numStr.substr(0, halfNumLength))] += stoneOccurrences;
            newMap[stoll(numStr.substr(halfNumLength, halfNumLength))] += stoneOccurrences;
            continue;
        }

        // Rule #3
        newMap[stoneNumber * 2024ll] += stoneOccurrences;
        continue;
    }

    std::swap(m_StoneOccurrences, newMap);
}

int CChallenge_11::CountStones_Part1() const
{
    int stoneCount = 0;

    const Stone* currentStone = m_FirstStone;
    while (currentStone != nullptr)
    {
        ++stoneCount;
        currentStone = currentStone->m_NextStone;
    }

    return stoneCount;
}

long long CChallenge_11::CountStones_Part2() const
{
    long long stoneCount = 0;

    for (const pair<long long, long long>& pair : m_StoneOccurrences)
    {
        stoneCount += pair.second;
    }

    return stoneCount;
}

#ifdef _DEBUG
void CChallenge_11::PrintStoneArrangement_Part1() const
{
    const Stone* currentStone = m_FirstStone;
    while (currentStone != nullptr)
    {
        cout << currentStone->m_Number << " ";
        currentStone = currentStone->m_NextStone;
    }

    cout << endl;
}
#endif

#ifdef _DEBUG
void CChallenge_11::PrintStoneArrangement_Part2() const
{
    for (const pair<long long, long long>& pair : m_StoneOccurrences)
    {
        if (pair.second > 0)
            cout << pair.first << ":" << pair.second << " ";
    }

    cout << endl;
}
#endif

#include <Challenges/Challenge_01.h>
#include <Helpers/FileHelpers.h>



////////////////////////////////////////////////////////////////////////////////////////////////////
string const CChallenge_01::sm_inputFilePath = "Inputs/Input_Challenge_01.txt";



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_01::SetUp_FirstPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    for (const string& line : lines)
    {
        vector<string> lineSplits;
        FileHelper::SplitLine(line, "   ", lineSplits);

        m_LeftSet.insert(stoi(lineSplits[0]));
        m_RightSet.insert(stoi(lineSplits[1]));
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_01::Run_FirstPart()
{
    long long totalDistance = 0ll;

    multiset<int>::const_iterator leftIt = m_LeftSet.cbegin();
    multiset<int>::const_iterator rightIt = m_RightSet.cbegin();
    while (leftIt != m_LeftSet.cend())
    {
        totalDistance += abs((*rightIt++) - (*leftIt++));
    }

    cout << "The total distance between both lists is " << totalDistance << "." << endl;

    return EErrorCode::Success;
}

EErrorCode CChallenge_01::CleanUp_FirstPart()
{
    m_LeftSet.clear();
    m_RightSet.clear();
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_01::SetUp_SecondPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    for (const string& line : lines)
    {
        vector<string> lineSplits;
        FileHelper::SplitLine(line, "   ", lineSplits);

        m_LeftSet.insert(stoi(lineSplits[0]));
        m_RightSet.insert(stoi(lineSplits[1]));
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_01::Run_SecondPart()
{
    long long similarityScore = 0ll;

    multiset<int>::const_iterator leftIt = m_LeftSet.cbegin();
    multiset<int>::const_iterator rightIt = m_RightSet.cbegin();
    while (leftIt != m_LeftSet.cend() && rightIt != m_RightSet.cend())
    {
        const int& leftID = *leftIt;
        while (rightIt != m_RightSet.cend() && (*rightIt) < leftID)
        {
            ++rightIt;
        }

        if (leftID == (*rightIt))
        {
            while (rightIt != m_RightSet.cend() && (*rightIt) == leftID)
            {
                similarityScore += leftID;
                ++rightIt;
            }
        }

        ++leftIt;
    }

    cout << "The similarity score between both lists is " << similarityScore << "." << endl;

    return EErrorCode::Success;
}

EErrorCode CChallenge_01::CleanUp_SecondPart()
{
    m_LeftSet.clear();
    m_RightSet.clear();
    return EErrorCode::Success;
}

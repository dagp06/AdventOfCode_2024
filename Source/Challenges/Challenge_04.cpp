#include "Challenge_04.h"
#include "../Helpers/FileHelpers.h"



////////////////////////////////////////////////////////////////////////////////////////////////////
string const CChallenge_04::sm_inputFilePath = "Inputs/Input_Challenge_04.txt";

////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_04::SetUp_FirstPart()
{
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, m_WordSearch);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_04::Run_FirstPart()
{
    constexpr const char* word = "XMAS";
    long long xmasCount = CountWord(word);

    cout << "There are " << xmasCount << " occurrences of " << word << " in the word search." << endl;
    return EErrorCode::Success;
}

EErrorCode CChallenge_04::CleanUp_FirstPart()
{
    m_WordSearch.clear();
    return EErrorCode::Success;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_04::SetUp_SecondPart()
{
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, m_WordSearch);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_04::Run_SecondPart()
{
    vector<string> pattern = { {"M.S"}, {".A."}, {"M.S"} };
    int x_masCount = CountPattern(pattern);

    pattern = { {"M.M"}, {".A."}, {"S.S"} };
    x_masCount += CountPattern(pattern);

    pattern = { {"S.M"}, {".A."}, {"S.M"} };
    x_masCount += CountPattern(pattern);

    pattern = { {"S.S"}, {".A."}, {"M.M"} };
    x_masCount += CountPattern(pattern);

    cout << "There are " << x_masCount << " X-MAS in the word search." << endl;
    return EErrorCode::Success;
}

EErrorCode CChallenge_04::CleanUp_SecondPart()
{
    m_WordSearch.clear();
    return EErrorCode::Success;
}

int CChallenge_04::CountWord(const string& word) const
{
    int wordCount = 0ll;

    struct DirectionDelta
    {
        int x = 0;
        int y = 0;
    };

    DirectionDelta directionDeltas[8] = {
        {-1, -1}, // Up-Left
        { 0, -1}, // Up 
        { 1, -1}, // Up-Right
        { 1,  0}, // Right
        { 1,  1}, // Down-Right
        { 0,  1}, // Down
        {-1,  1}, // Down-Left
        {-1,  0}  // Left
    };

    for (int row = 0; row < m_WordSearch.size(); ++row)
    {
        const string& line = m_WordSearch[row];

        for (int col = 0; col < line.size(); ++col)
        {
            const char& letter = line[col];

            // Start searching when first letter of the word is found
            if (letter != word[0])
                continue;

            for (const DirectionDelta& directionDelta : directionDeltas)
            {
                bool wordFound = true;
                for (int wordIdx = 1; wordIdx < word.size(); ++wordIdx)
                {
                    int localRow = row + wordIdx * directionDelta.y;
                    int localCol = col + wordIdx * directionDelta.x;

                    if (!ValidateRowCol(localRow, localCol))
                    {
                        wordFound = false;
                        break;
                    }

                    if (word[wordIdx] != m_WordSearch[localRow][localCol])
                    {
                        wordFound = false;
                        break;
                    }
                }

                if (wordFound)
                    ++wordCount;
            }
        }
    }

    return wordCount;
}

int CChallenge_04::CountPattern(const vector<string>& pattern) const
{
    // Current limitation - Only works if first char of pattern isn't '.'.
    int patternCount = 0ll;

    for (int row = 0; row < m_WordSearch.size(); ++row)
    {
        const string& line = m_WordSearch[row];

        for (int col = 0; col < line.size(); ++col)
        {
            const char& letter = line[col];

            // Start searching when first letter of the pattern is found
            if (letter != pattern[0][0])
                continue;

            bool patternFound = true;
            for (int patternIdxY = 0; patternIdxY < pattern.size(); ++patternIdxY)
            {
                for (int patternIdxX = 0; patternIdxX < pattern[0].size(); ++patternIdxX)
                {
                    const char& patternChar = pattern[patternIdxY][patternIdxX];
                    if (patternChar == '.')
                        continue;

                    int localRow = row + patternIdxY;
                    int localCol = col + patternIdxX;

                    if (!ValidateRowCol(localRow, localCol))
                    {
                        patternFound = false;
                        break;
                    }

                    const char& wordSearchChar = m_WordSearch[localRow][localCol];
                    if (wordSearchChar != patternChar)
                    {
                        patternFound = false;
                        break;
                    }
                }

                if (patternFound == false)
                    break;
            }

            if (patternFound)
                ++patternCount;
        }
    }

    return patternCount;
}

bool CChallenge_04::ValidateRowCol(int row, int col) const
{
    return row >= 0 && col >= 0 && row < m_WordSearch.size() && col < m_WordSearch[0].size();
}
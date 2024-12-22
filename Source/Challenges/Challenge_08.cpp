#include <Challenges/Challenge_08.h>
#include <Helpers/FileHelpers.h>

#include <set>

////////////////////////////////////////////////////////////////////////////////////////////////////
string const CChallenge_08::sm_inputFilePath = "Inputs/Input_Challenge_08.txt";



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_08::SetUp_FirstPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    m_RowCount = (int)lines.size();
    m_ColCount = (int)lines[0].size();
    for (int row = 0; row < m_RowCount; ++row)
    {
        for (int col = 0; col < m_ColCount; ++col)
        {
            const char& c = lines[row][col];
            if (isalnum(c))
            {
                m_FrequencyPos[c].push_back(Position(col, row));
            }
        }
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_08::Run_FirstPart()
{
    set<Position> antinodes;

    for (FrequencyPosMap::const_iterator it = m_FrequencyPos.begin(); it != m_FrequencyPos.end(); ++it)
    {
        const vector<Position>& posList = it->second;
        for (int i = 0; i < posList.size() - 1; ++i)
        {
            const Position& pos = posList[i];
            for (int j = i + 1; j < posList.size(); ++j)
            {
                const Position& otherPos = posList[j];

                Position antinode = pos + (pos - otherPos);
                if (IsPosInMappedArea(antinode))
                    antinodes.insert(antinode);

                antinode = otherPos + (otherPos - pos);
                if (IsPosInMappedArea(antinode))
                    antinodes.insert(antinode);
            }
        }
    }

    cout << "There are " << antinodes.size() << " unique locations with antinodes." << endl;

    return EErrorCode::Success;
}

EErrorCode CChallenge_08::CleanUp_FirstPart()
{
    m_FrequencyPos.clear();
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_08::SetUp_SecondPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    m_RowCount = (int)lines.size();
    m_ColCount = (int)lines[0].size();
    for (int row = 0; row < m_RowCount; ++row)
    {
        for (int col = 0; col < m_ColCount; ++col)
        {
            const char& c = lines[row][col];
            if (isalnum(c))
            {
                m_FrequencyPos[c].push_back(Position(col, row));
            }
        }
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_08::Run_SecondPart()
{
    set<Position> antinodes;

    for (FrequencyPosMap::const_iterator it = m_FrequencyPos.begin(); it != m_FrequencyPos.end(); ++it)
    {
        const vector<Position>& posList = it->second;
        if (posList.size() == 1)
            continue;

        for (int i = 0; i < posList.size(); ++i)
        {
            const Position& pos = posList[i];
            antinodes.insert(pos);

            for (int j = i + 1; j < posList.size(); ++j)
            {
                const Position& otherPos = posList[j];

                Position antinode = pos;

                Position posDiff = (pos - otherPos);
                while (true)
                {
                    antinode = antinode + posDiff;

                    if (!IsPosInMappedArea(antinode))
                        break;

                    antinodes.insert(antinode);
                }

                antinode = otherPos;
                posDiff = (otherPos - pos);
                while (true)
                {
                    antinode = antinode + posDiff;

                    if (!IsPosInMappedArea(antinode))
                        break;

                    antinodes.insert(antinode);
                }
            }
        }
    }

    cout << "There are " << antinodes.size() << " unique locations with antinodes." << endl;

    return EErrorCode::Success;
}

EErrorCode CChallenge_08::CleanUp_SecondPart()
{
    m_FrequencyPos.clear();
    return EErrorCode::Success;
}

bool CChallenge_08::IsPosInMappedArea(const Position& pos) const
{
    return pos.x >= 0 && pos.x < m_ColCount &&
        pos.y >= 0 && pos.y < m_RowCount;
}

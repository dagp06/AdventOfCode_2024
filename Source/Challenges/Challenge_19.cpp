#include <Challenges/Challenge_19.h>
#include <Helpers/FileHelpers.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
string const CChallenge_19::sm_inputFilePath = "Inputs/Input_Challenge_19.txt";

////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_19::SetUp_FirstPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    vector<string> towelsStr;
    FileHelper::SplitLine(lines[0], ", ", towelsStr);
    m_Towels.reserve(towelsStr.size());
    for (const string& towelStr : towelsStr)
    {
        m_Towels.push_back(towelStr);
    }

    m_Designs.reserve(lines.size() - 2);
    for (int i = 2; i < lines.size(); ++i)
    {
        m_Designs.push_back(lines[i]);
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_19::Run_FirstPart()
{
    int possibleDesignCount = 0;

    set<string> possibleDesigns;
    set<string> impossibleDesigns;
    for (const string& design : m_Designs)
    {
        possibleDesigns.clear();
        impossibleDesigns.clear();
        if (IsDesignPossible(design, possibleDesigns, impossibleDesigns))
        {
            ++possibleDesignCount;
#ifdef _DEBUG
            cout << "YES - " << design << endl;
        }
        else
        {
            cout << "NO  - " << design << endl;
#endif // _DEBUG
        }
    }

#ifndef BENCHMARK
    cout << "There are " << possibleDesignCount << " possible designs." << endl;
#endif // BENCHMARK

    return EErrorCode::Success;
}

EErrorCode CChallenge_19::CleanUp_FirstPart()
{
    m_Towels.clear();
    m_Designs.clear();
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_19::SetUp_SecondPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    vector<string> towelsStr;
    FileHelper::SplitLine(lines[0], ", ", towelsStr);
    m_Towels.reserve(towelsStr.size());
    for (const string& towelStr : towelsStr)
    {
        m_Towels.push_back(towelStr);
    }

    m_Designs.reserve(lines.size() - 2);
    for (int i = 2; i < lines.size(); ++i)
    {
        m_Designs.push_back(lines[i]);
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_19::Run_SecondPart()
{
    long long allArrangementCount = 0;

    DesignArrangementsMap designArrangements;
    set<string> impossibleDesigns;
    for (const string& design : m_Designs)
    {
        designArrangements.clear();
        impossibleDesigns.clear();

#ifdef _DEBUG
        long long designArrangementCount = CountPossibleArrangementsForDesign(design, designArrangements, impossibleDesigns);
        allArrangementCount += designArrangementCount;
        cout << designArrangementCount << " - " << design << endl;
#else
        allArrangementCount += CountPossibleArrangementsForDesign(design, designArrangements, impossibleDesigns);
#endif // _DEBUG
    }

#ifndef BENCHMARK
    cout << "There are " << allArrangementCount << " possible arrangements for all designs." << endl;
#endif // BENCHMARK

    return EErrorCode::Success;
}

EErrorCode CChallenge_19::CleanUp_SecondPart()
{
    m_Towels.clear();
    m_Designs.clear();
    return EErrorCode::Success;
}

bool CChallenge_19::IsDesignPossible(const string& _design, set<string>& _possibleDesigns, set<string>& _impossibleDesigns) const
{
    if (_impossibleDesigns.find(_design) != _impossibleDesigns.end())
        return false;

    if (_design.empty() || _possibleDesigns.find(_design) != _possibleDesigns.end())
        return true;

    for (const string& towel : m_Towels)
    {
        if (DoesDesignStartWith(_design, towel))
        {
            if (IsDesignPossible(_design.substr(towel.size()), _possibleDesigns, _impossibleDesigns))
            {
                _possibleDesigns.insert(_design);
                return true;
            }
        }
    }

    _impossibleDesigns.insert(_design);
    return false;
}


long long CChallenge_19::CountPossibleArrangementsForDesign(const string& _design, DesignArrangementsMap& _possibleDesignArrangements, set<string>& _impossibleDesigns) const
{
    if (_impossibleDesigns.find(_design) != _impossibleDesigns.end())
        return 0ll;

    if (_design.empty())
        return 1ll;

    DesignArrangementsMap::const_iterator it = _possibleDesignArrangements.find(_design);
    if (it != _possibleDesignArrangements.end())
        return it->second;

    bool isDesignPossible = false;
    for (const string& towel : m_Towels)
    {
        if (DoesDesignStartWith(_design, towel))
        {
            long long possibleArrangementsCount = CountPossibleArrangementsForDesign(_design.substr(towel.size()), _possibleDesignArrangements, _impossibleDesigns);
            if (possibleArrangementsCount > 0ll)
            {
                _possibleDesignArrangements[_design] += possibleArrangementsCount;
                isDesignPossible = true;
            }
        }
    }

    if (isDesignPossible)
        return _possibleDesignArrangements[_design];

    _impossibleDesigns.insert(_design);
    return 0ll;
}

bool CChallenge_19::DoesDesignStartWith(const string& _design, const string& _start) const
{
    for (int i = 0; i < _start.size(); ++i)
    {
        if (_start[i] != _design[i])
            return false;
    }

    return true;
}

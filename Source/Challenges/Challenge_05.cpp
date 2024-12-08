#include "Challenge_05.h"
#include "../Helpers/FileHelpers.h"

#include <algorithm>
#include <functional>

////////////////////////////////////////////////////////////////////////////////////////////////////
string const CChallenge_05::sm_inputFilePath = "Inputs/Input_Challenge_05.txt";



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_05::SetUp_FirstPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    int i = 0;
    for (; !lines[i].empty(); ++i)
    {
        const string& line = lines[i];

        vector<string> strOrderingRule;
        FileHelper::SplitLine(line, "|", strOrderingRule);

        int firstPage = stoi(strOrderingRule[0]);
        int secondPage = stoi(strOrderingRule[1]);
        m_MustPrecedeOrderingRules[firstPage].insert(secondPage);
        m_MustFollowOrderingRules[secondPage].insert(firstPage);
    }

    ++i; // Skip empty line
    m_PageUpdates.reserve(lines.size() - i);
    for (; i < lines.size(); ++i)
    {
        const string& line = lines[i];

        m_PageUpdates.emplace_back();
        PageUpdate& pageUpdate = m_PageUpdates.back();

        vector<string> strPageNumbers;
        FileHelper::SplitLine(line, ",", strPageNumbers);

        pageUpdate.reserve(strPageNumbers.size());
        for (const string& strPageNumber : strPageNumbers)
        {
            pageUpdate.push_back(stoi(strPageNumber));
        }
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_05::Run_FirstPart()
{
    int middlePageNumberSum = 0;

    for (const PageUpdate& pageUpdate : m_PageUpdates)
    {
        bool isPageUpdateValid = true;
        for (int i = 0; i < pageUpdate.size(); ++i)
        {
            const int& currentPage = pageUpdate[i];
            for (int j = i + 1; j < pageUpdate.size(); ++j)
            {
                const int& nextPage = pageUpdate[j];
                if (MustPrecede(nextPage, currentPage))
                {
                    isPageUpdateValid = false;
                    break;
                }
            }

            if (!isPageUpdateValid)
                break;
        }

        if (isPageUpdateValid)
            middlePageNumberSum += pageUpdate[pageUpdate.size() / 2];
    }

    cout << "The sum of the middle page number of valid updates is " << middlePageNumberSum << "." << endl;

    return EErrorCode::Success;
}

EErrorCode CChallenge_05::CleanUp_FirstPart()
{
    m_MustPrecedeOrderingRules.clear();
    m_MustFollowOrderingRules.clear();
    m_PageUpdates.clear();
    return EErrorCode::Success;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_05::SetUp_SecondPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    int i = 0;
    for (; !lines[i].empty(); ++i)
    {
        const string& line = lines[i];

        vector<string> strOrderingRule;
        FileHelper::SplitLine(line, "|", strOrderingRule);

        int firstPage = stoi(strOrderingRule[0]);
        int secondPage = stoi(strOrderingRule[1]);
        m_MustPrecedeOrderingRules[firstPage].insert(secondPage);
        m_MustFollowOrderingRules[secondPage].insert(firstPage);
    }

    ++i; // Skip empty line
    m_PageUpdates.reserve(lines.size() - i);
    for (; i < lines.size(); ++i)
    {
        const string& line = lines[i];

        m_PageUpdates.emplace_back();
        PageUpdate& pageUpdate = m_PageUpdates.back();

        vector<string> strPageNumbers;
        FileHelper::SplitLine(line, ",", strPageNumbers);

        pageUpdate.reserve(strPageNumbers.size());
        for (const string& strPageNumber : strPageNumbers)
        {
            pageUpdate.push_back(stoi(strPageNumber));
        }
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_05::Run_SecondPart()
{
    int middlePageNumberSum = 0;

    for (PageUpdate& pageUpdate : m_PageUpdates)
    {
        bool isPageUpdateValid = true;
        for (int i = 0; i < pageUpdate.size(); ++i)
        {
            const int& currentPage = pageUpdate[i];
            for (int j = i + 1; j < pageUpdate.size(); ++j)
            {

                const int& nextPage = pageUpdate[j];
                if (MustPrecede(nextPage, currentPage))
                {
                    isPageUpdateValid = false;
                    break;
                }
            }

            if (!isPageUpdateValid)
                break;
        }

        if (isPageUpdateValid)
            continue;

        auto comp = bind(&CChallenge_05::MustPrecede, this, placeholders::_1, placeholders::_2);
        sort(pageUpdate.begin(), pageUpdate.end(), comp);

        middlePageNumberSum += pageUpdate[pageUpdate.size() / 2];
    }

    cout << "The sum of the middle page number of sorted invalid updates is " << middlePageNumberSum << "." << endl;

    return EErrorCode::Success;
}

EErrorCode CChallenge_05::CleanUp_SecondPart()
{
    m_MustPrecedeOrderingRules.clear();
    m_MustFollowOrderingRules.clear();
    m_PageUpdates.clear();
    return EErrorCode::Success;
}


bool CChallenge_05::MustPrecede(const int& pageA, const int& pageB) const
{
    // Check for rules stating that pageA must precede pageB
    PageOrderingRuleSets::const_iterator itPrecedingRule = m_MustPrecedeOrderingRules.find(pageA);
    if (itPrecedingRule != m_MustPrecedeOrderingRules.end())
    {
        const set<int>& orderingRuleSet = itPrecedingRule->second;
        if (orderingRuleSet.find(pageB) != orderingRuleSet.end())
        {
            return true;
        }
    }

    // Check for rules stating that pageB must follow pageA
    PageOrderingRuleSets::const_iterator itFollowingRule = m_MustFollowOrderingRules.find(pageB);
    if (itFollowingRule != m_MustFollowOrderingRules.end())
    {
        const set<int>& orderingRuleSet = itFollowingRule->second;
        if (orderingRuleSet.find(pageA) != orderingRuleSet.end())
        {
            return true;
        }
    }

    return false;
}

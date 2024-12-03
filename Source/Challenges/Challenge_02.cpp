#include "Challenge_02.h"
#include "../Helpers/FileHelpers.h"



////////////////////////////////////////////////////////////////////////////////////////////////////
string const CChallenge_02::sm_inputFilePath = "Inputs/Input_Challenge_02.txt";



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_02::SetUp_FirstPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    m_Reports.resize(lines.size());
    for (int i = 0; i < lines.size(); ++i)
    {
        const string& line = lines[i];
        vector<string> lineSplits;
        FileHelper::SplitLine(line, " ", lineSplits);

        Report& report = m_Reports[i];
        report.resize(lineSplits.size());
        for (int i = 0; i < lineSplits.size(); ++i)
        {
            report[i] = stoi(lineSplits[i]);
        }
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_02::Run_FirstPart()
{
    int safeReportsCount = 0;

    for (const Report& report : m_Reports)
    {
        if (report.size() < 2)
        {
            ++safeReportsCount;
            continue;
        }

        bool isSafe = true;
        const bool isIncreasing = report[1] > report[0];
        int previousLevel = report[0];
        for (int i = 1; i < report.size(); ++i)
        {
            const int& currentLevel = report[i];
            const int delta = currentLevel - previousLevel;
            const int absDelta = abs(delta);

            if ((isIncreasing && delta < 0) || (!isIncreasing && delta > 0) || absDelta < 1 || absDelta > 3)
            {
                isSafe = false;
                break;
            }

            previousLevel = currentLevel;
        }

        safeReportsCount += isSafe ? 1 : 0;
    }

    cout << "There are " << safeReportsCount << " reports that are safe." << endl;
    return EErrorCode::Success;
}

EErrorCode CChallenge_02::CleanUp_FirstPart()
{
    m_Reports.clear();
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_02::SetUp_SecondPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    m_Reports.resize(lines.size());
    for (int i = 0; i < lines.size(); ++i)
    {
        const string& line = lines[i];
        vector<string> lineSplits;
        FileHelper::SplitLine(line, " ", lineSplits);

        Report& report = m_Reports[i];
        report.resize(lineSplits.size());
        for (int i = 0; i < lineSplits.size(); ++i)
        {
            report[i] = stoi(lineSplits[i]);
        }
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_02::Run_SecondPart()
{
    int safeReportsCount = 0;

    for (const Report& report : m_Reports)
    {
        safeReportsCount += IsReportSafe(report) ? 1 : 0;
    }

    cout << "There are " << safeReportsCount << " reports that are safe." << endl;
    return EErrorCode::Success;
}

EErrorCode CChallenge_02::CleanUp_SecondPart()
{
    m_Reports.clear();
    return EErrorCode::Success;
}

bool CChallenge_02::IsReportSafe(const Report& _report, bool _withTolerance /* = true */) const
{
    if (_report.size() < 2)
    {
        return true;
    }

    bool isSafe = true;
    int previousLevel = _report[0];
    const bool isIncreasing = _report[1] > _report[0];
    for (int i = 1; i < _report.size(); ++i)
    {
        const int& currentLevel = _report[i];
        const int delta = currentLevel - previousLevel;
        if (delta == 0)
        {
            if (!_withTolerance)
            {
                return false;
            }

            // Either level i, i - 1 or i - 2 could be bad.
            // Evaluate the report ignoring level i
            if (IsReportSafe(_report, i))
            {
                return true;
            }
            // Evaluate the report ignoring level i - 1
            else if (IsReportSafe(_report, i - 1))
            {
                return true;
            }
            // Evaluate the report ignoring level i - 2, if it exists
            else
            {
                if (i - 2 < 0)
                {
                    return false;
                }
                else
                {
                    return IsReportSafe(_report, i - 2);
                }
            }
        }

        const int absDelta = abs(delta);
        if ((isIncreasing && delta < 0) || (!isIncreasing && delta > 0) || absDelta < 1 || absDelta > 3)
        {
            if (!_withTolerance)
            {
                return false;
            }

            // Either level i, i - 1 or i - 2 could be bad.
            // Evaluate the report ignoring level i
            if (IsReportSafe(_report, i))
            {
                return true;
            }
            // Evaluate the report ignoring level i - 1
            else if (IsReportSafe(_report, i - 1))
            {
                return true;
            }
            // Evaluate the report ignoring level i - 2, if it exists
            else
            {
                if (i - 2 < 0)
                {
                    return false;
                }
                else
                {
                    return IsReportSafe(_report, i - 2);
                }
            }
        }

        previousLevel = currentLevel;
    }

    return true;
}

bool CChallenge_02::IsReportSafe(const Report& _report, int _indexToIgnore) const
{
    Report report = _report;
    report.erase(std::next(report.begin(), _indexToIgnore));
    return IsReportSafe(report, false);
}

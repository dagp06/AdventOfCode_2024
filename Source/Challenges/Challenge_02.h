#pragma once

#include <Challenges/Common/Challenge.h>

#include <vector>

using namespace std;

class CChallenge_02 : public CChallenge
{
private:
    typedef vector<int> Report;

    virtual EErrorCode SetUp_FirstPart() override;
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override;

    virtual EErrorCode SetUp_SecondPart() override;
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override;

    bool IsReportSafe(const Report& _report, bool _withTolerance = true) const;
    bool IsReportSafe(const Report& _report, int _indexToIgnore) const;

    static string const sm_inputFilePath;

    vector<Report> m_Reports;
};

#pragma once

#include <Challenges/Common/Challenge.h>
#include <Helpers/Position.h>

using namespace std;

class CChallenge_13 : public CChallenge
{
private:
    struct ClawMachine
    {
        Position m_DeltaA;
        Position m_DeltaB;
        Position m_PrizePos;

        bool Solve(unsigned long long& buttonAPressed, unsigned long long& buttonBPressed) const;
    };

    virtual EErrorCode SetUp_FirstPart() override;
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override;

    virtual EErrorCode SetUp_SecondPart() override;
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override;

    unsigned long long ComputeTokenSpent(const ClawMachine& _clawMachine);

    static string const sm_inputFilePath;
    static int const sm_buttonACost;
    static int const sm_buttonBCost;

    vector<ClawMachine> m_ClawMachines;
};

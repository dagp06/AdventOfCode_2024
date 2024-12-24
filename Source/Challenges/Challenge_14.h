#pragma once

#include <Challenges/Common/Challenge.h>
#include <Helpers/Vector2.h>

using namespace std;

class CChallenge_14 : public CChallenge
{
private:
    struct Robot
    {
        Vector2 pos;
        Vector2 velocity;
        void Move(int _secondsToElapse);
    };

    virtual EErrorCode SetUp_FirstPart() override;
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override;

    virtual EErrorCode SetUp_SecondPart() override;
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override;

    float ComputeEntropy() const;
    void DrawMap() const;

    static string const sm_inputFilePath;
    static Vector2 const sm_mapSize;

    vector<Robot> m_Robots;
    vector<vector<int>> m_RobotsMap;
};

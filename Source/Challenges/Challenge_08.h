#pragma once

#include "Common/Challenge.h"

#include <map>

using namespace std;

class CChallenge_08 : public CChallenge
{
private:
    struct Position
    {
        Position(int _x = 0, int _y = 0)
            : x(_x), y(_y) {}

        Position operator+(const Position& _other) const { return Position(x + _other.x, y + _other.y); }
        Position operator-(const Position& _other) const { return Position(x - _other.x, y - _other.y); }
        bool operator==(const Position& _other) const { return x == _other.x && y == _other.y; }
        bool operator<(const Position& _other) const { return x < _other.x || (x == _other.x && y < _other.y); }

        int x;
        int y;
    };

    virtual EErrorCode SetUp_FirstPart() override;
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override;

    virtual EErrorCode SetUp_SecondPart() override;
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override;

    bool IsPosInMappedArea(const Position& pos) const;

    static string const sm_inputFilePath;

    typedef map<char, vector<Position>> FrequencyPosMap;
    FrequencyPosMap m_FrequencyPos;
    int m_RowCount;
    int m_ColCount;
};

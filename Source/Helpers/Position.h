#pragma once
#include <CommonDefinitions.h>

#include <unordered_set>

using namespace std;

struct Position
{
    Position(int _x = 0, int _y = 0)
        : x(_x), y(_y) {}
    bool operator==(const Position& _other) const { return x == _other.x && y == _other.y; }
    bool operator<(const Position& _other) const { return x < _other.x || (x == _other.x && y < _other.y); }
    Position operator+(const Position& _other) const { return Position(x + _other.x, y + _other.y); }
    Position operator-(const Position& _other) const { return *this + -_other; }
    Position operator-() const { return Position(-x, -y); }

    int x;
    int y;
};

struct PositionHash
{
    size_t operator()(const Position& _pos) const
    {
        return hash<int>()(_pos.x) ^ (hash<int>()(_pos.y) << 1);
    }
};

typedef unordered_set<Position, PositionHash> PositionSet;
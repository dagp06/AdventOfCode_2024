#pragma once
#include <CommonDefinitions.h>

#include <unordered_set>

using namespace std;

struct Position
{
    Position(long long _x = 0, long long _y = 0)
        : x(_x), y(_y) {}
    bool operator==(const Position& _other) const { return x == _other.x && y == _other.y; }
    bool operator<(const Position& _other) const { return x < _other.x || (x == _other.x && y < _other.y); }
    Position operator+(const Position& _other) const { return { x + _other.x, y + _other.y }; }
    Position operator-(const Position& _other) const { return *this + -_other; }
    Position operator-() const { return { -x, -y }; }

    friend Position operator*(long long scalar, const Position& _pos) { return { scalar * _pos.x, scalar * _pos.y }; }

    long long x = 0;
    long long y = 0;
};

struct PositionHash
{
    size_t operator()(const Position& _pos) const
    {
        return hash<long long>()(_pos.x) ^ (hash<long long>()(_pos.y) << 1);
    }
};

typedef unordered_set<Position, PositionHash> PositionSet;
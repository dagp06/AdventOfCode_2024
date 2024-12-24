#pragma once
#include <CommonDefinitions.h>

#include <unordered_set>

using namespace std;

struct Vector2
{
    Vector2(long long _x = 0, long long _y = 0)
        : x(_x), y(_y) {}
    bool operator==(const Vector2& _other) const { return x == _other.x && y == _other.y; }
    bool operator<(const Vector2& _other) const { return x < _other.x || (x == _other.x && y < _other.y); }
    Vector2 operator+(const Vector2& _other) const { return { x + _other.x, y + _other.y }; }
    Vector2 operator-(const Vector2& _other) const { return *this + -_other; }
    Vector2 operator-() const { return { -x, -y }; }

    Vector2 operator*(long long _scalar) const { return { _scalar * x, _scalar * y }; }
    friend Vector2 operator*(long long _scalar, const Vector2& _vec) { return { _vec * _scalar }; }

    Vector2 operator+=(const Vector2& _other) {
        x += _other.x;
        y += _other.y;
        return *this;
    }
    Vector2 operator-=(const Vector2& _other) {
        x -= _other.x;
        y -= _other.y;
        return *this;
    }

    long long x = 0;
    long long y = 0;
};

struct Vector2Hash
{
    size_t operator()(const Vector2& _vec) const
    {
        return hash<long long>()(_vec.x) ^ (hash<long long>()(_vec.y) << 1);
    }
};

typedef unordered_set<Vector2, Vector2Hash> Vector2Set;
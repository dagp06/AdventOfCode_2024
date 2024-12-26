#pragma once
#include <CommonDefinitions.h>

#include <Helpers/Vector2.h>

using namespace std;

enum class Direction
{
    Up,
    Right,
    Down,
    Left,
    Count
};

namespace DirectionUtils {
    Vector2 GetDelta(Direction _dir);
    Direction GetOpposite(Direction _dir);
    Direction GetNext(Direction _dir, bool _autoCycle = true);
}
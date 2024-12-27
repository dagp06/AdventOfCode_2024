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

ostream& operator<<(ostream& _stream, Direction _dir);

namespace DirectionUtils {
    Vector2 GetDelta(Direction _dir);
    Direction GetOpposite(Direction _dir);
    Direction GetPrev(Direction _dir, bool _autoCycle = true);
    Direction GetNext(Direction _dir, bool _autoCycle = true);
}
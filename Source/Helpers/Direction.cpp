#pragma once
#include <Helpers/Direction.h>

namespace DirectionUtils {
    static const Vector2 sm_directionDeltas[] = { {0, -1}, {1, 0}, {0, 1}, {-1, 0} };

    Vector2 GetDelta(Direction _dir)
    {
        return sm_directionDeltas[(int)_dir];
    }

    Direction GetOpposite(Direction _dir)
    {
        return (Direction)(((int)_dir + 2) % (int)Direction::Count);
    }

    Direction GetNext(Direction _dir, bool _autoCycle /* = true */)
    {
        return _autoCycle ? (Direction)(((int)_dir + 1) % (int)Direction::Count) : (Direction)((int)_dir + 1);
    }
}
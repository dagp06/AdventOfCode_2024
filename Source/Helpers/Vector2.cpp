#pragma once
#include <Helpers/Vector2.h>

std::ostream& operator<<(ostream& _stream, Vector2 _vec)
{
    return _stream << "(" << _vec.x << "," << _vec.y << ")";
}

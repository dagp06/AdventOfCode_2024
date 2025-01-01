#pragma once
#include <Helpers/Vector2.h>

const Vector2 Vector2::ZERO{ 0, 0 };

std::ostream& operator<<(ostream& _stream, Vector2 _vec)
{
    return _stream << "(" << _vec.x << "," << _vec.y << ")";
}

#include "Vector.h"

#include <climits>

namespace Nova
{
	Vector2::Vector2() :
		Vector2(0.0, 0.0)
	{}

	Vector2::Vector2(double x, double y) :
		X(x), Y(y)
	{}

	const Vector2 Vector2::ZERO = Vector2(0.0, 0.0);

	Vector2i::Vector2i() :
		Vector2i(0, 0)
	{}

	Vector2i::Vector2i(int x, int y) :
		X(x), Y(y)
	{}

	const Vector2i Vector2i::ZERO = Vector2i(0, 0);
	const Vector2i Vector2i::MAX = Vector2i(INT_MAX, INT_MAX);

	bool Vector2i::operator==(const Vector2i& lhs) const
	{
		return this->X == lhs.X && this->Y == lhs.Y;
	}
}
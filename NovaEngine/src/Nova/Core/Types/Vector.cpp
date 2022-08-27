#include "Vector.h"

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

	bool Vector2i::operator==(const Vector2i& lhs) const
	{
		return this->X == lhs.X && this->Y == lhs.Y;
	}
}
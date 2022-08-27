#include "Rect.h"
#include "Vector.h"

namespace Nova
{
	Rect::Rect() :
		Rect(Vector2::ZERO, Vector2::ZERO)
	{}

	Rect::Rect(const Vector2& size, const Vector2& position) :
		Size(size), Position(position)
	{}

	Recti::Recti() :
		Recti(Vector2i::ZERO, Vector2i::ZERO)
	{}

	Recti::Recti(const Vector2i& size, const Vector2i& position) :
		Size(size), Position(position)
	{}

	bool Recti::operator==(const Recti& other)
	{
		return this->Size == other.Size && this->Position == other.Position;
	}
}
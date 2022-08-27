#pragma once

#include "Nova/Core/EngineAPI.h"
#include "Vector.h"

namespace Nova
{
	/// <summary>
	/// A rectangle that has a size and a position with decimal coordinates
	/// </summary>
	struct NovaAPI Rect
	{
		Rect();
		Rect(const Vector2& size, const Vector2& position);

		virtual ~Rect() = default;

		Vector2 Size;
		Vector2 Position;
	};

	/// <summary>
	/// A rectangle that has a size and a position with integer coordinates
	/// </summary>
	struct NovaAPI Recti
	{
		Recti();
		Recti(const Vector2i& size, const Vector2i& position);

		virtual ~Recti() = default;

		Vector2i Size;
		Vector2i Position;

		virtual bool operator==(const Recti& other);
	};
}
#pragma once

#include "Nova/Core/EngineAPI.h"

namespace Nova
{
	/// <summary>
	/// A two-dimensional vector with decimal coordinates
	/// </summary>
	struct NovaAPI Vector2
	{
		Vector2();
		Vector2(double x, double y);

		virtual ~Vector2() = default;

		double X, Y;

		static const Vector2 ZERO;
	};

	/// <summary>
	/// A two-dimensional vector with integer coordinates
	/// </summary>'
	struct NovaAPI Vector2i
	{
		Vector2i();
		Vector2i(int x, int y);

		virtual ~Vector2i() = default;

		int X, Y;

		static const Vector2i ZERO;
		static const Vector2i MAX;

		bool operator==(const Vector2i& lhs) const;
	};
}
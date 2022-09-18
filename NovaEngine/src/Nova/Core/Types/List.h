#pragma once

#include <vector>

namespace Nova
{
	// TODO: create our own List class for stable API

	/// <summary>
	/// A resizable array
	/// </summary>
	template<typename T>
	using List = std::vector<T>;
}
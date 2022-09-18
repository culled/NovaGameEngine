#pragma once

#include<map>

namespace Nova
{
	// TODO: create our own Map class for stable API

	/// <summary>
	/// Represents a key-value pair list
	/// </summary>
	template<typename KeyType, typename ValueType>
	using Map = std::map<KeyType, ValueType>;
}
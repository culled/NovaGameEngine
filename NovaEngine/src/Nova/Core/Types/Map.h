#pragma once

#include<map>

namespace Nova
{
	template<typename KeyType, typename ValueType>
	using Map = std::map<KeyType, ValueType>;
}
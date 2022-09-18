#pragma once

#include "Nova/Core/EngineAPI.h"
#include "Nova/Core/Types/String.h"
#include <exception>

namespace Nova
{
	/// <summary>
	/// Base exception type
	/// </summary>
	class NovaAPI Exception : public std::exception
	{
	public:
		Exception(const string& message);
		virtual ~Exception() = default;

	public:
		static Exception GetException();
	};
}
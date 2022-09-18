#pragma once

#include "Nova/Core/EngineAPI.h"
#include "Nova/Core/Types/Exception.h"

namespace Nova
{
	/// <summary>
	/// An exception for when a window wasn't able to be created
	/// </summary>
	class NovaAPI WindowCreateException : public Exception
	{
	public:
		WindowCreateException(const string& error);
	};
}
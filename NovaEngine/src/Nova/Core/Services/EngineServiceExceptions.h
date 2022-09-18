#pragma once

#include "Nova/Core/EngineAPI.h"

#include "Nova/Core/Types/Exception.h"
#include "Nova/Core/Types/String.h"

namespace Nova
{
	/// <summary>
	/// Represents an exception initializing an AppModule
	/// </summary>
	class NovaAPI EngineServiceInitException : public Exception
	{
	public:
		EngineServiceInitException(const string& error);
	};
}
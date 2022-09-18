#pragma once

#include "Nova/Core/EngineAPI.h"
#include "Nova/Core/Types/Exception.h"
#include "Nova/Core/Types/String.h"

namespace Nova
{
	class NovaAPI EngineInitException : public Exception
	{
	public:
		EngineInitException(const string& what);
	};
}
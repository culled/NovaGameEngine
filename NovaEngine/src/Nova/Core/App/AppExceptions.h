#pragma once

#include "Nova/Core/EngineAPI.h"
#include "Nova/Core/Types/Exception.h"

namespace Nova
{
	class NovaAPI AppInitException : public Exception
	{
	public:
		AppInitException(const string& what);
	};
}
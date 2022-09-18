#pragma once

#include "Nova/Core/EngineAPI.h"
#include "Nova/Core/Types/Exception.h"

namespace Nova
{
	/// <summary>
	/// An exception for when the WindowingBackend fails to initialize
	/// </summary>
	class NovaAPI WindowingBackendInitException : public Exception
	{
	public:
		WindowingBackendInitException(const string& error);
	};
}
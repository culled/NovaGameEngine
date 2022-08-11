#pragma once

#include "Nova/Core/Engine.h"

#include "Nova/Core/Types/Exception.h"

namespace Nova
{
	/// <summary>
	/// Represents an exception initializing an AppModule
	/// </summary>
	NovaClass AppModuleInitException : public Exception
	{
	public:
		AppModuleInitException(const string& error);

		virtual ~AppModuleInitException() = default;
	};
}
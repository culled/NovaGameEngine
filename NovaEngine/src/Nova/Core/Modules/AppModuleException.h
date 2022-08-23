#pragma once

#include "Nova/Core/Engine.h"

#include "Nova/Core/Types/Exception.h"

namespace Nova
{
	/// <summary>
	/// Represents an exception initializing an AppModule
	/// </summary>
	class NovaAPI AppModuleInitException : public Exception
	{
	public:
		AppModuleInitException(const string& error);

		virtual ~AppModuleInitException() = default;
	};
}
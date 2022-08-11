#pragma once

#include "Nova/Core/Types/String.h"

namespace Nova
{
	/// <summary>
	/// Enum of log levels
	/// </summary>
	enum class LogLevel
	{
		Verbose,
		Info,
		Warning,
		Error
	};

	/// <summary>
	/// Gets the string representation of a log level
	/// </summary>
	/// <param name="level">The level</param>
	/// <returns>The level as a string</returns>
	string LogLevelAsString(LogLevel level);
}
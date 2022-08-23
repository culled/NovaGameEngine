#include "LogLevel.h"

namespace Nova
{
	/// <summary>
	/// Gets the string representation of a log level
	/// </summary>
	/// <param name="level">The level</param>
	/// <returns>The level as a string</returns>
	string LogLevelAsString(LogLevel level)
	{
		switch (level)
		{
		case LogLevel::Info:
			return "Info";
		case LogLevel::Warning:
			return "Warning";
		case LogLevel::Error:
			return "Error";
		default:
			return "Verbose";
		}
	}
}
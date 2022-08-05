// Base class of a sink that receives log messages

#pragma once

#include "Nova/Core/Engine.h"
#include "LogLevel.h"

namespace Nova
{
	NovaClass LogSink
	{
	public:
		LogSink() = default;
		virtual ~LogSink() = default;

		/// <summary>
		/// Writes to this sink
		/// </summary>
		/// <param name="message">The message to write</param>
		/// <param name="level">The log level of the message</param>
		virtual void Write(const string& message, LogLevel level) = 0;
	};
}
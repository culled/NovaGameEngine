#pragma once

#include "Nova/Core/EngineAPI.h"

#include "Nova/Core/Types/String.h"
#include "LogSink.h"

namespace Nova
{
	/// <summary>
	/// A log sink that prints messages to a console window
	/// </summary>
	class NovaAPI ConsoleLogSink : public LogSink
	{
	public:
		ConsoleLogSink(LogLevel minimumLevel = LogLevel::Verbose);

		virtual ~ConsoleLogSink() = default;

	// LogSink ----------
	protected:
		virtual void WriteImpl(const string& message, LogLevel level) override;

	// LogSink ----------
	};
}
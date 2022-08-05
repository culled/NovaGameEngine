#pragma once

#include "Nova/Core/Engine.h"
#include "Nova/Core/Types/String.h"
#include "LogSink.h"

namespace Nova
{
	NovaClass ConsoleLogSink : public LogSink
	{
	public:
		ConsoleLogSink();
		virtual ~ConsoleLogSink() = default;

		virtual void Write(const string & message, LogLevel level) override;
	};
}
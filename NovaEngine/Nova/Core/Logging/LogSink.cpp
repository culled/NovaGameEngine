#include "LogSink.h"

namespace Nova
{
	LogSink::LogSink(LogLevel minimumLevel) :
		m_MinimumLevel(minimumLevel)
	{}

	bool LogSink::ShouldWrite(LogLevel level) const
	{
		return level >= m_MinimumLevel;
	}

	void LogSink::Write(const string& message, LogLevel level)
	{
		if (!ShouldWrite(level))
			return;

		WriteImpl(message, level);
	}
}
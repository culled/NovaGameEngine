#include "Logger.h"

#include "Nova/Core/App/App.h"
#include "Nova/Core/Types/DateTime.h"

#include <format>

namespace Nova
{
	Logger::Logger(const string& name) :
		m_Name(name)
	{}

	void Logger::Write(const string& message, LogLevel level)
	{
		TimeSpan currentTime = App::GetRunningTime();

		string logMessage = std::format("[{0:02d}:{1:02d}.{2:04d}] {3}: {4}", currentTime.GetMinutes(), currentTime.GetSeconds(), currentTime.GetMilliseconds(), m_Name, message);
		for (const auto& sink : m_Sinks)
		{
			sink->Write(logMessage, level);
		}
	}

	void Logger::AddSink(Ref<LogSink> sink)
	{
		m_Sinks.push_back(sink);
	}
}
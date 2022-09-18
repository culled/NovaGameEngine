#include "Logger.h"

#include "Nova/Core/App/App.h"
#include "Nova/Core/Types/DateTime.h"
#include "Nova/Core/Engine/Engine.h"

#include <format>

namespace Nova
{
	Logger::Logger(const string& name) :
		m_Name(name)
	{}

	void Logger::AddSink(const Ref<LogSink>& sink)
	{
		m_Sinks.push_back(sink);
	}

	void Logger::Write(const string& message, LogLevel level)
	{
		// TODO: support message as a format (provide variable number of arguments)

		TimeSpan currentTime = Engine::Get()->GetRunningTime();

		// Format the message
		string logMessage = std::format("[{0:02d}:{1:02d}.{2:04d} - {3}] {4}: {5}", 
			currentTime.GetMinutes(),
			currentTime.GetSeconds(),
			currentTime.GetMilliseconds(),
			LogLevelAsString(level),
			m_Name,
			message);

		// Write the message to our sinks
		for (const auto& sink : m_Sinks)
		{
			sink->Write(logMessage, level);
		}
	}
}
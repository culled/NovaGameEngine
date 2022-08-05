#include "App.h"

#include "Nova/Core/Logging/ConsoleLogSink.h"

namespace Nova
{
	App* App::sp_AppInstance = nullptr;

	App::App(const string& name) :
		m_Name(name), m_StartTime(TimeSpan::Now())
	{
		sp_AppInstance = this;

		m_CoreLogger = MakeRef<Logger>("Nova");
		m_AppLogger = MakeRef<Logger>(name);
	}

	App::~App()
	{}

	TimeSpan App::GetRunningTime()
	{
		return TimeSpan::Now() - GetStartTime();
	}

	TimeSpan App::GetStartTime()
	{
		return sp_AppInstance->m_StartTime;
	}

	void App::Log(const string& message, LogLevel level)
	{
		sp_AppInstance->m_AppLogger->Write(message, level);
	}

	void App::LogCore(const string& message, LogLevel level)
	{
		sp_AppInstance->m_CoreLogger->Write(message, level);
	}

	void App::CreateDefaultLogSinks()
	{
		m_CoreLogger->CreateSink<ConsoleLogSink>();
		m_AppLogger->CreateSink<ConsoleLogSink>();

		LogCore("Core logger created");
		Log("App logger created");
	}
}
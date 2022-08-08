#include "App.h"

#include "Nova/Core/Logging/ConsoleLogSink.h"

namespace Nova
{
	App* App::sp_AppInstance = nullptr;

	App::App(const string& name) :
		m_Name(name), m_StartTime(TimeSpan::Now()), m_ExitCode(AppExitCode::SUCCESS)
	{
		sp_AppInstance = this;

		// Create our default logs
		m_CoreLogger = MakeRef<Logger>("Nova");
		m_AppLogger = MakeRef<Logger>(name);

		// Create the main application loop
		m_MainLoop = CreateMainLoop();

		// Create the main node tree and add it to the loop
		m_NodeTree = MakeRef<NodeTree>();
		m_MainLoop->AddTickListener(m_NodeTree);
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

		LogCore("App::CreateDefaultLogSinks(): Core logger created", LogLevel::Verbose);
		Log("App::CreateDefaultLogSinks(): App logger created", LogLevel::Verbose);
	}

	Ref<MainLoop> App::CreateMainLoop()
	{
		return MakeRef<MainLoop>();
	}

	App::AppExitCode App::Run()
	{
		LogCore("App::Run(): Starting MainLoop", LogLevel::Verbose);
		m_MainLoop->Start();

		return m_ExitCode;
	}
}
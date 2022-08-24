#include "App.h"

#include "Nova/Core/Logging/ConsoleLogSink.h"

namespace Nova
{
	App::App(const string& name) :
		m_Name(name), m_StartTime(TimeSpan::Now()), m_ExitCode(AppExitCode::SUCCESS)
	{
		sp_AppInstance = this;

		// Create our default logs
		m_CoreLogger = MakeExclusive<Logger>("Nova");
		m_AppLogger = MakeExclusive<Logger>(name);

		// Create the main application loop
		m_MainLoop = CreateMainLoop();

		// Create the main node tree and add it to the loop
		m_NodeTree = MakeRef<NodeTree>();
		m_MainLoop->AddTickListener(m_NodeTree);
	}

	App::~App()
	{
		// Release all modules
		m_AppModules.clear();
	}

	TimeSpan App::GetRunningTime()
	{
		return TimeSpan::Now() - GetStartTime();
	}

	TimeSpan App::GetStartTime()
	{
		return sp_AppInstance->m_StartTime;
	}

	double App::GetDeltaTime()
	{
		return sp_AppInstance->m_MainLoop->GetDeltaTime();
	}

	App* App::sp_AppInstance = nullptr;

	App::AppExitCode App::Run()
	{
		LogCore(LogLevel::Verbose, "App::Run(): Starting MainLoop");

		try
		{
			m_MainLoop->Start();

			OnAppQuit.EmitAnonymous();

			return m_ExitCode;
		}
		catch (...)
		{
			Exception ex = GetException();

			// TODO: error handle for app
			LogCore(LogLevel::Error, "An unhandled exception occured: {0}", ex.what());

			return AppExitCode::UNHANDLED_EXCEPTION;
		}
	}

	bool App::Quit(AppExitCode exitCode)
	{
		AppQuittingEvent quittingEvent;
		OnAppQuitting.Emit(quittingEvent);

		if (quittingEvent.ShouldQuit)
		{	
			m_ExitCode = exitCode;
			m_MainLoop->Stop();

			return true;
		}
		
		return false;
	}

	void App::CreateDefaultLogSinks(LogLevel coreLevel, LogLevel appLevel)
	{
		m_CoreLogger->CreateSink<ConsoleLogSink>(coreLevel);
		m_AppLogger->CreateSink<ConsoleLogSink>(appLevel);

		LogCore(LogLevel::Verbose, "App::CreateDefaultLogSinks(): Core logger created");
		Log(LogLevel::Verbose, "App::CreateDefaultLogSinks(): App logger created");
	}

	Exclusive<MainLoop> App::CreateMainLoop()
	{
		return MakeExclusive<MainLoop>();
	}
}
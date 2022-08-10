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
		m_CoreLogger = MakeExclusive<Logger>("Nova");
		m_AppLogger = MakeExclusive<Logger>(name);

		// Create the main application loop
		m_MainLoop = CreateMainLoop();

		// Create the main node tree and add it to the loop
		m_NodeTree = MakeRef<NodeTree>();
		//m_NodeTree->CreateRootNode();
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

	Exclusive<MainLoop> App::CreateMainLoop()
	{
		return MakeExclusive<MainLoop>();
	}

	void App::AddModule(Ref<AppModule> appModule)
	{
		m_AppModules.push_back(appModule);
		m_MainLoop->AddTickListener(appModule);
	}

	App::AppExitCode App::Run()
	{
		LogCore("App::Run(): Starting MainLoop", LogLevel::Verbose);
		m_MainLoop->Start();

		return m_ExitCode;
	}

	void App::Quit()
	{
		Ref<AppQuittingEvent> quittingEvent = MakeRef<AppQuittingEvent>();
		OnAppQuitting.Emit(quittingEvent);

		if (quittingEvent->ShouldQuit)
		{
			OnAppQuit.Emit(MakeRef<Event>());
			m_MainLoop->Stop();
		}
	}
}
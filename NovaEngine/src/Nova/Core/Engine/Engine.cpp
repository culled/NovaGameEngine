#include "Engine.h"
#include "EngineExceptions.h"
#include "Nova/Core/Logging/ConsoleLogSink.h"

/// The function that will create our client's App class. Should be defined using the MainApp macro
extern Nova::App* CreateAppInstance(const Nova::List<Nova::string>& args);

namespace Nova
{
	Engine::Engine(const List<string>& args) :
		m_StartupArgs(args),
		m_StartTime(DateTime::Now()),
		m_Logger(new Logger("Nova")),
		m_MainLoop(new MainLoop()),
		m_ExitCode(AppExitCode::NONE)
	{
		m_Logger->CreateSink<ConsoleLogSink>(LogLevel::Verbose);

		CreateSingleton();
		CreateApp();
	}

	Engine::~Engine()
	{
		// Cleanup the app before we exit
		m_App.reset();
		m_Services.clear();
		m_MainLoop.reset();

		Log(LogLevel::Verbose, "Engine destroyed");
	}

	Engine* Engine::Init(const List<string>& args)
	{
		// Create a new engine
		Engine* engine = new Engine(args);
		return engine;
	}

	Engine* Engine::s_Instance = nullptr;

	AppExitCode Engine::Run()
	{
		// Exit if something happened during init and we should exit
		if (m_ExitCode != AppExitCode::NONE)
		{
			return m_ExitCode;
		}

		try
		{
			m_MainLoop->Run();

			return m_ExitCode;
		}
		catch (...)
		{
			Exception ex = Exception::GetException();

			Log(LogLevel::Error, "An unhandled {0} exception occurred while running the application: {1}", typeid(ex).name(), ex.what());

			Stop(AppExitCode::UNHANDLED_EXCEPTION);

			return AppExitCode::UNHANDLED_EXCEPTION;
		}
	}

	void Engine::Stop(AppExitCode exitCode)
	{
		m_MainLoop->Stop();
		SetExitCode(exitCode);
		OnStop.EmitAnonymous();
	}

	TimeSpan Engine::GetRunningTime() const
	{
		return DateTime::Now() - m_StartTime;
	}

	void Engine::CreateSingleton()
	{
		if (s_Instance)
			throw EngineInitException("Another instance of the engine already exists!");

		s_Instance = this;

		Log(LogLevel::Verbose, "Engine singleton created");
	}

	void Engine::CreateApp()
	{
		try
		{
			m_App.reset(CreateAppInstance(m_StartupArgs));
		}
		catch (const EngineServiceInitException& ex)
		{
			Log(LogLevel::Error, "Error creating engine service: {0}", ex.what());

			// Services are vital, so shutdown if one fails to initialize
			Stop(AppExitCode::SUCCESS);

			return;
		}
	}
}
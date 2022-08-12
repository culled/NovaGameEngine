// Base class for the main application

#pragma once

#include "Nova/Core/Engine.h"

#include "Nova/Core/Modules/AppModule.h"
#include "Nova/Core/Events/Event.h"
#include "Nova/Core/Events/EventSource.h"
#include "Nova/Core/Types/DateTime.h"
#include "Nova/Core/Logging/Logger.h"
#include "Nova/Core/Nodes/NodeTree.h"
#include "MainLoop.h"

namespace Nova
{
	NovaStruct AppQuittingEvent : public Event
	{
		/// <summary>
		/// Gets/sets if the app should go through with the quit
		/// </summary>
		bool ShouldQuit = true;
	};

	NovaClass App
	{
	public:
		/// <summary>
		/// Possible exit codes for the app
		/// </summary>
		enum class AppExitCode
		{
			// The app exited normally
			SUCCESS = 0,

			// The app exited with an unhandled exception
			UNHANDLED_EXCEPTION = 1
		};

	public:
		/// <summary>
		/// Constructor for this application
		/// </summary>
		/// <param name="name">The name for this application</param>
		App(const string& name);

		/// <summary>
		/// The destructor for this application
		/// </summary>
		virtual ~App();

	public:
		/// <summary>
		/// Gets the duration of time this application has been running
		/// </summary>
		/// <returns>A TimeSpan that represents the amount of time this app has been running</returns>
		static TimeSpan GetRunningTime();

		/// <summary>
		/// Gets the time that this application was started
		/// </summary>
		/// <returns>A TimeSpan that represents the time when this app was started</returns>
		static TimeSpan GetStartTime();

		/// <summary>
		/// Logs to the app's logger
		/// </summary>
		/// <param name="message">The message to log</param>
		/// <param name="level">The log level of the message</param>
		static void Log(const string& message, LogLevel level = LogLevel::Info);

		/// <summary>
		/// Logs to the core engine logger
		/// </summary>
		/// <param name="message">The message to log</param>
		/// <param name="level">The log level of the message</param>
		static void LogCore(const string& message, LogLevel level = LogLevel::Info);

		/// <summary>
		/// Gets the instance of the app
		/// </summary>
		/// <returns>The app instance</returns>
		static App* Get() { return sp_AppInstance; }

	private:
		// The instance of the application
		static App* sp_AppInstance;

	public:
		/// <summary>
		/// The function that will run all the tasks for this application. Once this returns the app is shutdown
		/// </summary>
		virtual AppExitCode Run();

		/// <summary>
		/// Attempts to quit the application
		/// </summary>
		/// <param name="exitCode">The exit code to exit with if successful</param>
		virtual void Quit(AppExitCode exitCode = AppExitCode::SUCCESS);

		/// <summary>
		/// Loads an AppModule into this application
		/// </summary>
		/// <typeparam name="T">The type of module to load</typeparam>
		/// <param name="executionOffset">A user-defined offset to apply on top of the module's default offset</param>
		/// <param name="...args">Any extra parameters to pass to the constructor of the module class</param>
		template<class T, typename ... Args>
		Ref<T> CreateAndAddModule(int executionOffset = 0, Args&& ...args)
		{
			// Only accept classes that derive from AppModule
			static_assert(std::is_base_of<AppModule, T>::value, "The class must inherit from AppModule");

			// Create the app module and pass the given arguments
			Ref<T> appModule = MakeRef<T>(executionOffset, std::forward<Args>(args)...);

			// Add the created module to our list of modules and register it to the main loop
			m_AppModules.push_back(appModule);
			m_MainLoop->AddTickListener(appModule);

			return appModule;
		}

		/// <summary>
		/// Gets the name of this application
		/// </summary>
		/// <returns>The name of this application</returns>
		const string& GetName() const { return m_Name; }

		/// <summary>
		/// Gets the node tree for the application
		/// </summary>
		/// <returns>The app's node tree</returns>
		Ref<NodeTree> GetNodeTree() const { return m_NodeTree; }

	protected:
		/// <summary>
		/// Creates ConsoleLogSinks for the core and app loggers
		/// </summary>
		/// <param name="coreLevel">The minimum level to write core log messages</param>
		/// <param name="appLevel">The minimum level to write app log messages</param>
		void CreateDefaultLogSinks(LogLevel coreLevel, LogLevel appLevel);

		/// <summary>
		/// Creates the MainLoop for the application. By default returns an instance of the MainLoop class
		/// </summary>
		/// <returns>An instance of a MainLoop</returns>
		virtual Exclusive<MainLoop> CreateMainLoop();

		/// <summary>
		/// Sets the exit code for the app that will be returned when it quits
		/// </summary>
		/// <param name="exitCode">The exit code</param>
		void SetExitCode(AppExitCode exitCode) { m_ExitCode = exitCode; }

	public:
		/// <summary>
		/// Event that is invoked when the app is requested to quit
		/// </summary>
		EventSource<AppQuittingEvent> OnAppQuitting;

		/// <summary>
		/// Event that is invoked when the app is quitting
		/// </summary>
		EventSource<Event> OnAppQuit;

	protected:
		// Gets the name of the application
		const string m_Name;

		// Gets the time the app was started
		const TimeSpan m_StartTime;

	private:
		// The code the app will exit with
		AppExitCode m_ExitCode;

		// The logger for the core of Nova (non-user logs)
		Exclusive<Logger> m_CoreLogger;

		// The logger for the app
		Exclusive<Logger> m_AppLogger;

		// The main loop of the app
		Exclusive<MainLoop> m_MainLoop;

		// A list of all active modules for the app
		List<Ref<AppModule>> m_AppModules;

		// The main node tree for the app
		Ref<NodeTree> m_NodeTree;
	};
}

// Macro to create an App factory that returns an instance of the client's app class
#define MainApp(AppClass) Nova::App* CreateApp(const Nova::List<Nova::string>& args) { return new AppClass(args); }
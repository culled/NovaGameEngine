// Base class for the main application

#pragma once

#include "Nova/Core/Engine.h"
#include "Nova/Core/Modules/AppModule.h"
#include "Nova/Core/Types/DateTime.h"
#include "Nova/Core/Logging/Logger.h"
#include "Nova/Core/Nodes/NodeTree.h"
#include "MainLoop.h"

namespace Nova
{
	NovaClass App
	{
	public:
		enum class AppExitCode
		{
			SUCCESS = 0,
			UNHANDLED_EXCEPTION = 1
		};

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
		static App* sp_AppInstance;

	public:
		/// <summary>
		/// Constructor for this application
		/// </summary>
		/// <param name="name">The name for this application</param>
		App(const string& name);

		/// <summary>
		/// The destructor for this application
		/// </summary>
		virtual ~App() = default;

		/// <summary>
		/// The function that will run all the tasks for this application. Once this returns the app is shutdown
		/// </summary>
		virtual AppExitCode Run();

		/// <summary>
		/// Gets the name of this application
		/// </summary>
		/// <returns>The name of this application</returns>
		const string& GetName() const { return m_Name; }

		/// <summary>
		/// Loads an AppModule into this application
		/// </summary>
		/// <typeparam name="T">The type of module to load</typeparam>
		/// <param name="executionOffset">A user-defined offset to apply on top of the module's default offset</param>
		/// <param name="...args">Any extra parameters to pass to the constructor of the module class</param>
		template<class T, typename ... Args>
		void CreateModule(int executionOffset = 0, Args&& ...args)
		{
			// Only accept classes that derive from AppModule
			static_assert(std::is_base_of<AppModule, T>::value, "The class must inherit from AppModule");

			Ref<T> appModule = MakeRef<T>(executionOffset, std::forward<Args>(args)...);

			AddModule(appModule);
		}

		void AddModule(Ref<AppModule> appModule);

	protected:
		/// <summary>
		/// Creates ConsoleLogSinks for the core and app loggers
		/// </summary>
		void CreateDefaultLogSinks();

		/// <summary>
		/// Creates the MainLoop for the application. By default returns an instance of the MainLoop class
		/// </summary>
		/// <returns>An instance of a MainLoop</returns>
		virtual Exclusive<MainLoop> CreateMainLoop();

	protected:
		string m_Name;
		AppExitCode m_ExitCode;

		TimeSpan m_StartTime;

		Exclusive<Logger> m_CoreLogger;
		Exclusive<Logger> m_AppLogger;

		Exclusive<MainLoop> m_MainLoop;
		List<Ref<AppModule>> m_AppModules;
		Ref<NodeTree> m_NodeTree;
	};
}

// Macro to create an App factory that returns an instance of the client's app class
#define MainApp(AppClass) Nova::App* CreateApp(const Nova::List<Nova::string>& args) { return new AppClass(args); }
// Base class for the main application

#pragma once

#include "Nova/Core/EngineAPI.h"

#include "Nova/Core/Types/RefCounted.h"
#include "Nova/Core/Events/Event.h"
#include "Nova/Core/Events/EventSource.h"
#include "Nova/Core/Logging/Logger.h"
#include "Nova/Core/Nodes/NodeTree.h"
#include "Nova/Core/Events/Event.h"
#include "Nova/Core/Engine/AppExitCode.h"

#include <type_traits>

namespace Nova
{
	/// <summary>
	/// Event for when the app is requested to quit
	/// </summary>
	struct NovaAPI AppQuittingEvent : public Event
	{
		/// <summary>
		/// Constructs an AppQuittingEvent
		/// </summary>
		/// <param name="forceQuitting">If true, then the app will quit regardless if Cancel is true</param>
		AppQuittingEvent(bool forceQuitting);

		/// <summary>
		/// Gets if the app will quit regardless if ShouldQuit is true
		/// </summary>
		const bool ForceQuitting;

		/// <summary>
		/// If set to true, the app will cancel its quit (unless ForceQuitting is true)
		/// </summary>
		bool Cancel;
	};

	/// <summary>
	/// Base class for an application that is run by the engine
	/// </summary>
	class NovaAPI App
	{
	public:
		/// <summary>
		/// Constructor for this application
		/// </summary>
		/// <param name="name">The name for this application</param>
		App(const string& name);
		virtual ~App();

	public:
		/// <summary>
		/// Gets the instance of the app
		/// </summary>
		/// <returns>The app instance</returns>
		static App* Get() { return s_Instance; }

	private:
		/// <summary>
		/// The instance of the application
		/// </summary>
		static App* s_Instance;

	public:
		/// <summary>
		/// Attempts to quit the application
		/// </summary>
		/// <param name="exitCode">The exit code to exit with if successful</param>
		/// <param name="forceQuit">If true, the application will quit regardless if the quit is cancelled</param>
		/// <returns>True if the app will quit</returns>
		virtual bool Quit(AppExitCode exitCode = AppExitCode::SUCCESS, bool forceQuit = false);

		/// <summary>
		/// Logs to the app's logger
		/// </summary>
		/// <param name="level">The log level of the message</param>
		/// <param name="formatMessage">The message to log</param>
		/// <param name="values">Values to be integrated into the formatted message</param>
		template <typename ... Args>
		void Log(LogLevel level, const string& formatMessage, Args&& ... values)
		{
			m_Logger->WriteFormatted(level, formatMessage, std::forward<Args>(values)...);
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

	private:
		/// <summary>
		/// Creates the app singleton
		/// </summary>
		void CreateSingleton();

		/// <summary>
		/// Creates the NodeTree for this application
		/// </summary>
		void CreateNodeTree();

	public:
		/// <summary>
		/// Event that is invoked when the app is requested to quit
		/// </summary>
		EventSource<AppQuittingEvent> OnAppQuitting;

	protected:
		/// <summary>
		/// Gets the name of this application
		/// </summary>
		const string m_Name; 

	private:
		/// <summary>
		/// The logger for the app
		/// </summary>
		ManagedPtr<Logger> m_Logger;

		/// <summary>
		/// The main node tree of the application
		/// </summary>
		Ref<NodeTree> m_NodeTree;
	};
}

/// Macro to create an App factory that returns an instance of the client's app class
#define MainApp(AppClass) Nova::App* CreateAppInstance(const Nova::List<Nova::string>& args) { return new AppClass(args); }
#include "App.h"

#include "Nova/Core/Engine/Engine.h"
#include "Nova/Core/Logging/ConsoleLogSink.h"
#include "AppExceptions.h"

namespace Nova
{
	AppQuittingEvent::AppQuittingEvent(bool forceQuitting) :
		ForceQuitting(forceQuitting), Cancel(false)
	{}

	App::App(const string& name) :
		m_Name(name), m_Logger(new Logger(name))
	{
		m_Logger->CreateSink<ConsoleLogSink>();

		CreateSingleton();
		CreateNodeTree();
	}

	App::~App()
	{
		// Cleanup the NodeTree
		m_NodeTree.reset();

		Log(LogLevel::Verbose, "App destroyed");
	}

	App* App::s_Instance = nullptr;

	bool App::Quit(AppExitCode exitCode, bool forceQuit)
	{
		// Broadcast a quitting event to everybody
		AppQuittingEvent quittingEvent(forceQuit);
		OnAppQuitting.Emit(quittingEvent);

		if (forceQuit || !quittingEvent.Cancel)
		{	
			Engine::Get()->Stop(exitCode);

			return true;
		}
		
		return false;
	}

	void App::CreateSingleton()
	{
		if (s_Instance)
			throw AppInitException("Another instance of the app already exists!");

		s_Instance = this;

		Log(LogLevel::Verbose, "App singleton created");
	}

	void App::CreateNodeTree()
	{
		m_NodeTree = MakeRef<NodeTree>();
	}
}
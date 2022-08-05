// This represents the main entrypoint for the engine. It's recommended to include this in your app's class that derives from App

#pragma once

#include "Nova/Core/Engine.h"
#include "Nova/Core/App/App.h"
#include "Nova/Core/Types/List.h"
#include "Nova/Core/Types/String.h"
#include "Nova/Core/Types/Exception.h"

#ifdef PLATFORM_WINDOWS
#include "Nova/Platform/Windows/Core/Entry_Windows.h"
#endif


// Macro to create an App factory that returns an instance of the client's app class
#define MainApp(AppClass) Nova::App* CreateApp(const Nova::List<Nova::string>& args) { return new AppClass(args); }

// The function that will create our client's App class. Should be defined using the MainApp macro
extern Nova::App* CreateApp(const Nova::List<Nova::string>& args);

namespace Nova
{
	/// <summary>
	/// This is the main entrypoint for the application. It is called from the platform-specific entrypoints.
	/// </summary>
	/// <param name="args">A list of execution arguments</param>
	/// <returns>Application exit code</returns>
	int Entry(const Nova::List<Nova::string>& args)
	{
		auto app = CreateApp(args);

		try
		{
			app->Run();
		}
		catch(const Exception& ex)
		{
			// TODO: error handle for app
		}

		delete app;

		return 0;
	}
}
#pragma once

#include "Nova/Core/Engine.h"
#include "Nova/Core/App/App.h"

#ifndef NOVA_EXTERNAL_MAIN
#ifdef PLATFORM_WINDOWS
#include "Nova/Platform/Windows/Core/Entry_Windows.h"
#endif
#endif

/// The function that will create our client's App class. Should be defined using the MainApp macro
extern Nova::App* CreateApp(const Nova::List<Nova::string>& args);

namespace Nova
{
	/// <summary>
	/// This is the main entrypoint for the application. It is called from the platform-specific entrypoints.
	/// </summary>
	/// <param name="args">A list of execution arguments</param>
	/// <returns>Application exit code</returns>
	App::AppExitCode Entry(const Nova::List<Nova::string>& args)
	{
		std::unique_ptr<App> app;
		App::AppExitCode exitCode;

		try
		{
			app.reset(CreateApp(args));
		}
		catch (...)
		{
			// TODO: handle app unable to start
			return App::AppExitCode::UNHANDLED_EXCEPTION;
		}

		exitCode = app->Run();

		return exitCode;
	}
}
#pragma once

#include "Engine.h"
#include "Nova/Core/Types/RefCounted.h"
#include "Nova/Core/Types/Exception.h"

#include <type_traits>

#ifndef NOVA_EXTERNAL_MAIN
#ifdef PLATFORM_WINDOWS
#include "Nova/Platform/Windows/Core/Entry_Windows.h"
#endif
#endif

namespace Nova
{
	/// <summary>
	/// This is the main entrypoint for the engine. It's called from the platform-specific entrypoints.
	/// </summary>
	/// <param name="args">A list of execution arguments</param>
	/// <returns>The exit code of the application</returns>
    AppExitCode Entry(const Nova::List<Nova::string>& args)
	{
		ManagedPtr<Engine> engine;

		try
		{
			// Create the engine instance
			engine.reset(Engine::Init(args));
			
			// Run our application
			return engine->Run();
		}
		catch (...)
		{
			// Get the exception that was thrown
			Exception ex = Exception::GetException();

			engine->Log(LogLevel::Error, "An unhandled {0} exception occurred: {1}", typeid(ex).name(), ex.what());

			return AppExitCode::UNHANDLED_EXCEPTION;
		}
	}
}
#pragma once

namespace Nova
{
	/// <summary>
	/// Possible exit codes for the app
	/// </summary>
	enum class AppExitCode
	{
		// The default code when the app is not ready to exit
		NONE = -999,

		// The app exited normally
		SUCCESS = 0,

		// The app exited with an unhandled exception
		UNHANDLED_EXCEPTION = 1,
	};
}
#include "TestApp.h"

#include <catch.hpp>

#define NOVA_EXTERNAL_MAIN // Prevent Nova from defining a main function
#include <Nova/Core/App/Entry.h>

Nova::List<Nova::string> TestApp::s_Args = { "Arg1", "Arg2" };

TEST_CASE("Nova/Core/App/App Lifecycle", "Check if app forwards arguments and returns the proper exit code")
{
	Nova::App::AppExitCode exitCode = Nova::Entry(TestApp::s_Args);

	REQUIRE(exitCode == Nova::App::AppExitCode::SUCCESS);
}
#pragma once

#include <catch.hpp>
#include <Nova/Core/App/App.h>

class TestApp : public Nova::App
{
public:
	static Nova::List<Nova::string> s_Args;

public:
	TestApp(Nova::List<Nova::string> args) : Nova::App("Test App")
	{
		// Check if name is set correctly
		REQUIRE(m_Name == "Test App");

		// Check if the passed in args are correct
		REQUIRE(args.size() == s_Args.size());

		for (size_t i = 0; i < args.size(); i++)
		{
			REQUIRE(args[i] == s_Args[i]);
		}
	}

	virtual Nova::App::AppExitCode Run() override { return AppExitCode::SUCCESS; }
};

MainApp(TestApp)
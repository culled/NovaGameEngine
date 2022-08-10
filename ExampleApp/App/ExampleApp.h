#pragma once

// Entrypoint ------------
#include <Nova/Core/App/Entry.h>
// Entrypoint ------------

#include <Nova/Core/Engine.h>
#include <Nova/Core/App/App.h>

class ExampleApp : public Nova::App
{
public:
	ExampleApp(const Nova::List<Nova::string>& args);
	~ExampleApp();

	//virtual Nova::App::AppExitCode Run() override;
};

// Define this as the application class that will be created for the lifetime of the app
MainApp(ExampleApp)
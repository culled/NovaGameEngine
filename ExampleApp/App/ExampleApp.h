#pragma once

// Entrypoint ------------
#include <Nova/Core/App/Entry.h>
// Entrypoint ------------

#include <Nova/Core/App/App.h>
#include <Nova/Core/Types/List.h>
#include <Nova/Core/Types/String.h>

class ExampleApp : public Nova::App
{
public:
	ExampleApp(const Nova::List<Nova::string>& args);
	~ExampleApp();

	virtual void Run() override;
};

// Define this as the application class that will be created for the lifetime of the app
MainApp(ExampleApp)
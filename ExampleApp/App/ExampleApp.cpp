#include "ExampleApp.h"
#include <Nova/Core/Logging/ConsoleLogSink.h>


ExampleApp::ExampleApp(const Nova::List<Nova::string>& args) : App("Example App")
{
	CreateDefaultLogSinks();

	Log("ExampleApp created");
}

ExampleApp::~ExampleApp()
{
	Log("ExampleApp destroyed");
}

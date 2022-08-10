#include "ExampleApp.h"
#include <Nova/Core/Logging/ConsoleLogSink.h>
#include <Nova/Core/Modules/Display/GLFW/GLFWDisplayModule.h>


ExampleApp::ExampleApp(const Nova::List<Nova::string>& args) : App("Example App")
{
	CreateDefaultLogSinks();

	Log("ExampleApp created");

	auto display = CreateAndAddModule<Nova::Display::GLFWDisplayModule>();
	display->CreateAndAddWindow(1280, 720, "Example App");
}

ExampleApp::~ExampleApp()
{
	Log("ExampleApp destroyed");
}

//Nova::App::AppExitCode ExampleApp::Run()
//{
//	return Nova::App::AppExitCode::SUCCESS;
//}

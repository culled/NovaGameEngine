#include "ExampleApp.h"
#include <Nova/Core/Logging/ConsoleLogSink.h>
#include <Nova/Core/Modules/Windowing/GLFW/GLFWWindowingModule.h>


ExampleApp::ExampleApp(const Nova::List<Nova::string>& args) : App("Example App")
{
	CreateDefaultLogSinks(Nova::LogLevel::Verbose, Nova::LogLevel::Verbose);

	Log("ExampleApp created");

	OnAppQuitting.Connect(this, &ExampleApp::OnQuitting);

	auto display = CreateAndAddModule<Nova::Windowing::GLFWWindowingModule>();

	Nova::Windowing::WindowCreateParams createParams(1280, 720, "Example App");
	display->CreateAndAddWindow(createParams);
}

ExampleApp::~ExampleApp()
{
	Log("ExampleApp destroyed");
}

void ExampleApp::OnQuitting(Nova::AppQuittingEvent& e)
{
	//e.ShouldQuit = false;
	//
	Log("I'm quitting!");
}

//Nova::App::AppExitCode ExampleApp::Run()
//{
//	return Nova::App::AppExitCode::SUCCESS;
//}

#include "ExampleApp.h"
#include <Nova/Core/Logging/ConsoleLogSink.h>

#include <Nova/Core/Modules/Rendering/RenderModule.h>
#include <Nova/Core/Modules/Windowing/GLFW/GLFWWindowingModule.h>


ExampleApp::ExampleApp(const Nova::List<Nova::string>& args) : App("Example App")
{
	CreateDefaultLogSinks(Nova::LogLevel::Verbose, Nova::LogLevel::Verbose);

	Log(Nova::LogLevel::Verbose, "ExampleApp created");

	OnAppQuitting.Connect(this, &ExampleApp::OnQuitting);

	CreateAndAddModule<Nova::Rendering::RenderModule>(0, Nova::Rendering::RenderingBackendAPI::OpenGL);

	auto display = CreateAndAddModule<Nova::Windowing::GLFWWindowingModule>();

	Nova::Windowing::WindowCreateParams createParams(1280, 720, "Example App");
	display->CreateAndAddWindow(createParams);
}

ExampleApp::~ExampleApp()
{
	Log(Nova::LogLevel::Verbose, "ExampleApp destroyed");
}

void ExampleApp::OnQuitting(Nova::AppQuittingEvent& e)
{
	//e.ShouldQuit = false;
	//
	Log(Nova::LogLevel::Info, "I'm quitting!");
}

//Nova::App::AppExitCode ExampleApp::Run()
//{
//	return Nova::App::AppExitCode::SUCCESS;
//}

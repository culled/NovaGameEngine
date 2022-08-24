#include "ExampleApp.h"
#include <Nova/Core/Logging/ConsoleLogSink.h>

#include <Nova/Core/Modules/Rendering/RenderModule.h>
#include <Nova/Core/Modules/Windowing/WindowingModule.h>
#include <Nova/ImGui/ImGuiRenderLayer.h>
#include <imgui.h>

ExampleApp::ExampleApp(const Nova::List<Nova::string>& args) : App("Example App")
{
	CreateDefaultLogSinks(Nova::LogLevel::Verbose, Nova::LogLevel::Verbose);

	Log(Nova::LogLevel::Verbose, "ExampleApp created");

	OnAppQuitting.Connect(this, &ExampleApp::OnQuitting);

	auto renderer = CreateAndAddModule<Nova::Rendering::RenderModule>(0, Nova::Rendering::RenderingBackendAPI::OpenGL);
	auto windowing = CreateAndAddModule<Nova::Windowing::WindowingModule>(0, Nova::Windowing::WindowingAPI::GLFW);

	Nova::Windowing::WindowCreateParams createParams(1280, 720, "Example App");
	windowing->CreateAndAddWindow(createParams);

	// We have to add our ImGui layer after creating our window because no OpenGL context exists until a window is created
	auto imGuiLayer = Nova::MakeRef<Nova::ImGui::ImGuiRenderLayer>();
	imGuiLayer->OnBeginFrame.Connect(this, &ExampleApp::OnImGuiDraw);
	renderer->AppendRenderLayer(imGuiLayer);
}

ExampleApp::~ExampleApp()
{
	Log(Nova::LogLevel::Verbose, "ExampleApp destroyed");
}

void ExampleApp::OnQuitting(Nova::AppQuittingEvent& e)
{
	Log(Nova::LogLevel::Info, "I'm quitting!");
}

void ExampleApp::OnImGuiDraw(Nova::ImGui::ImGuiRenderEvent& e)
{
	bool open = true;
	ImGui::ShowDemoWindow(&open);

	ImGui::Begin("Info");

	int fps = (int)std::lround(1.0f / e.DeltaTime);
	ImGui::Text("%dfps", fps);

	ImGui::End();
}

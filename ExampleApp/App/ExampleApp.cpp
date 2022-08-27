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

	// Setup our rendering
	auto renderer = CreateAndAddModule<Nova::Rendering::RenderModule>(0, Nova::Rendering::RenderingBackendAPI::OpenGL);

	// Setup our windowing
	auto windowing = CreateAndAddModule<Nova::Windowing::WindowingModule>(0, Nova::Windowing::WindowingAPI::GLFW);

	// Create an ImGui layer
	auto imGuiLayer = Nova::MakeRef<Nova::ImGui::ImGuiRenderLayer>();
	renderer->AddRenderLayer(imGuiLayer);

	// Create our main window
	Nova::Windowing::WindowCreateParams createParams(Nova::Vector2i(1280, 720), "Example App");
	auto mainWindow = windowing->CreateWindow(createParams).lock();

	// Use our main window with ImGui
	mainWindow->GetGraphicsContext()->AddLayerRenderingListener((int)Nova::Rendering::DefaultRenderLayerIDs::ImGui, this, &ExampleApp::OnImGuiLayerRender);
	imGuiLayer->CreateContextForWindow(mainWindow);

	Nova::Windowing::WindowCreateParams createSecondParams(Nova::Vector2i(640, 480), "Second Window");
	createSecondParams.ShareGfxContext = mainWindow->GetGraphicsContext();
	
	auto secondWindow = windowing->CreateWindow(createSecondParams).lock();
	
	secondWindow->GetGraphicsContext()->AddLayerRenderingListener((int)Nova::Rendering::DefaultRenderLayerIDs::ImGui, this, &ExampleApp::OnImGuiLayerRenderSecondWindow);
	imGuiLayer->CreateContextForWindow(secondWindow);
}

ExampleApp::~ExampleApp()
{
	Log(Nova::LogLevel::Verbose, "ExampleApp destroyed");
}

void ExampleApp::OnQuitting(Nova::AppQuittingEvent& e)
{
	Log(Nova::LogLevel::Info, "I'm quitting!");
}

void ExampleApp::OnImGuiLayerRender(Nova::Event& e)
{
	bool open = true;
	ImGui::ShowDemoWindow(&open);

	ImGui::Begin("Info");

	int fps = (int)std::lround(1.0f / GetDeltaTime());
	ImGui::Text("%dfps", fps);

	ImGui::End();
}

void ExampleApp::OnImGuiLayerRenderSecondWindow(Nova::Event& e)
{
	bool open = true;
	ImGui::ShowAboutWindow(&open);
}
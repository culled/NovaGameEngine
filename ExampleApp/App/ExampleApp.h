#pragma once

// Entrypoint ------------
#include <Nova/Core/Engine/Entry.h>
// Entrypoint ------------

//#include <Nova/Core/Engine.h>
#include <Nova/Core/App/App.h>
//#include <Nova/ImGui/ImGuiRenderLayer.h>

class ExampleApp : public Nova::App
{
public:
	ExampleApp(const Nova::List<Nova::string>& args);
	~ExampleApp();

	//virtual Nova::App::AppExitCode Run() override;

	void OnTreeTick(Nova::TickEvent& e);
	void OnQuitting(Nova::AppQuittingEvent& e);

	//void OnImGuiLayerRender(Nova::Event& e);
	//void OnImGuiLayerRenderSecondWindow(Nova::Event& e);
};

// Define this as the application class that will be created for the lifetime of the app
MainApp(ExampleApp)
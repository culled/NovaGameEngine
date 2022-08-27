#include "ImGuiOpenGLRenderer.h"

#include "Nova/Core/Modules/Windowing/Window.h"
#include "Nova/ImGui/imgui_impl_nova.h"

#include <backends/imgui_impl_opengl3.h>

namespace Nova::ImGui
{
	ImGuiOpenGLRenderer::~ImGuiOpenGLRenderer()
	{
		
		
	}

	// ImGuiRenderer ----------
	void ImGuiOpenGLRenderer::InitializeNewImGuiContext(Ref<Windowing::Window> window)
	{
		if (!ImGui_ImplNova_Init(window))
		{
			throw Exception("Could not initialize ImGui implementation");
		}

		window->GetGraphicsContext()->MakeCurrent();

		int imGuiOpenGLStatus = ImGui_ImplOpenGL3_Init("#version 460");
		if (!imGuiOpenGLStatus)
		{
			throw Exception("Could not initialize OpenGL for ImGui");
		}

		window->GetGraphicsContext()->ReleaseCurrent();
	}

	void ImGuiOpenGLRenderer::ShutdownImGuiContext(Ref<Windowing::Window> window)
	{
		ImGui_ImplNova_Shutdown();

		window->GetGraphicsContext()->MakeCurrent();

		ImGui_ImplOpenGL3_Shutdown();

		window->GetGraphicsContext()->ReleaseCurrent();
	}

	void ImGuiOpenGLRenderer::BeginFrame(Ref<Rendering::GraphicsContext> context, double deltaTime)
	{
		ImGui_ImplNova_NewFrame(context, deltaTime);
		ImGui_ImplOpenGL3_NewFrame();
	}

	void ImGuiOpenGLRenderer::RenderImGuiData()
	{
		ImGui_ImplOpenGL3_RenderDrawData(::ImGui::GetDrawData());
	}
	// ImGuiRenderer ----------
}
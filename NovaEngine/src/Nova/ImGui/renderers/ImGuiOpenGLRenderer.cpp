#include "ImGuiOpenGLRenderer.h"

#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"

namespace Nova::ImGui
{
	ImGuiOpenGLRenderer::ImGuiOpenGLRenderer()
	{
		ImGui_ImplOpenGL3_Init();
	}

	ImGuiOpenGLRenderer::~ImGuiOpenGLRenderer()
	{
		ImGui_ImplOpenGL3_Shutdown();
	}

	// ImGuiRenderer ----------
	void ImGuiOpenGLRenderer::BeginFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
	}

	void ImGuiOpenGLRenderer::EndFrame()
	{
		ImGui_ImplOpenGL3_RenderDrawData(::ImGui::GetDrawData());
	}
	// ImGuiRenderer ----------
}
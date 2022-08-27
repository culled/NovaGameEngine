#pragma once

#include "Nova/Core/Engine.h"
#include "Nova/ImGui/ImGuiRenderer.h"

namespace Nova::ImGui
{
	/// <summary>
	/// Implementation of ImGuiRenderer for OpenGL
	/// </summary>
	class NovaAPI ImGuiOpenGLRenderer : public ImGuiRenderer
	{
	public:
		~ImGuiOpenGLRenderer();

	// ImGuiRenderer ----------
	public:
		virtual void InitializeNewImGuiContext(Ref<Windowing::Window> window) override;
		virtual void ShutdownImGuiContext(Ref<Windowing::Window> window) override;

		virtual void BeginFrame(Ref<Rendering::GraphicsContext> context, double deltaTime) override;
		virtual void RenderImGuiData() override;

	// ImGuiRenderer ----------
	};
}
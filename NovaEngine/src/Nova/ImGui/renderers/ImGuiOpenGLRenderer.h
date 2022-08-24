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
		ImGuiOpenGLRenderer();
		~ImGuiOpenGLRenderer();

	// ImGuiRenderer ----------
	public:
		virtual void BeginFrame() override;
		virtual void EndFrame() override;

	// ImGuiRenderer ----------
	};
}
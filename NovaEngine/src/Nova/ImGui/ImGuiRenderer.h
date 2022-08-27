#pragma once

#include "Nova/Core/Engine.h"
#include "Nova/Core/Modules/Rendering/GraphicsContext.h"

namespace Nova::Windowing
{
	class Window;
}

namespace Nova::ImGui
{
	/// <summary>
	/// Base class of an ImGui rendering backend
	/// </summary>
	class NovaAPI ImGuiRenderer
	{
	public:
		virtual ~ImGuiRenderer() = default;

	public:
		/// <summary>
		/// Called when an ImGuiContext is created for the given window
		/// </summary>
		/// <param name="window">The window</param>
		virtual void InitializeNewImGuiContext(Ref<Windowing::Window> window) = 0;

		/// <summary>
		/// Called when an ImGuiContext is shutdown for the given window
		/// </summary>
		/// <param name="window">The window</param>
		virtual void ShutdownImGuiContext(Ref<Windowing::Window> window) = 0;

		/// <summary>
		/// Begins rendering for a context
		/// </summary>
		/// <param name="context">The context to render</param>
		/// <param name="deltaTime">The time between the last frame and this frame (in seconds)</param>
		virtual void BeginFrame(Ref<Rendering::GraphicsContext> context, double deltaTime) = 0;

		/// <summary>
		/// Renders ImGui data to the active GraphicsContext
		/// </summary>
		virtual void RenderImGuiData() = 0;
	};
}
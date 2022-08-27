#pragma once

#include "Nova/Core/Engine.h"
#include "Nova/Core/Modules/Rendering/Layers/RenderLayer.h"
#include "Nova/Core/Modules/Rendering/GraphicsContext.h"
#include "Nova/Core/Modules/Windowing/Window.h"

#include "ImGuiRenderer.h"

namespace Nova::Windowing
{
	struct WindowDestroyedEvent;
}

struct ImGuiContext;

namespace Nova::ImGui
{
	class NovaAPI ImGuiRenderLayer : public Rendering::RenderLayer
	{
	public:
		ImGuiRenderLayer();
		~ImGuiRenderLayer();

		// RefCounted ----------
	protected:
		virtual void Init() override;

		// RefCounted ----------
		
		// RenderLayer ----------
	public:
		virtual void BeginFrame(double deltaTime) override;
		virtual void EndFrame() override;
		virtual void RenderContext(Ref<Rendering::GraphicsContext> context) override;

		virtual int GetID() const override { return static_cast<int>(Rendering::DefaultRenderLayerIDs::ImGui); }

		// RenderLayer ----------

		/// <summary>
		/// Creates an ImGui context for a window
		/// </summary>
		/// <param name="window">The window to use</param>
		void CreateContextForWindow(Ref<Windowing::Window> window);

	private:
		/// <summary>
		/// Creates the internal ImGui renderer depending on the current RenderingAPI
		/// </summary>
		void CreateRenderer();

		/// <summary>
		/// Gets an ImGuiContext for a given GraphicsContext
		/// </summary>
		/// <param name="context">The context</param>
		/// <returns>An ImGuiContext if one has been created for the given context</returns>
		ImGuiContext* GetImGuiContextForGraphicsContext(Ref<Rendering::GraphicsContext> context) const;

		/// <summary>
		/// Called when the WindowingModule destroys a window
		/// </summary>
		/// <param name="e">The event params</param>
		void WindowDestroyedCallback(Windowing::WindowDestroyedEvent& e);

	private:
		/// <summary>
		/// The ImGuiRenderer
		/// </summary>
		Exclusive<ImGuiRenderer> m_Renderer;

		/// <summary>
		/// A map that links windows to their ImGui contexts
		/// </summary>
		Map<Ref<Windowing::Window>, ImGuiContext*> m_WindowContextMap;

		/// <summary>
		/// The delta time for the current frame
		/// </summary>
		double m_DeltaTime;
	};
}
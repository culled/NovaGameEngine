#pragma once

#include "Nova/Core/Engine.h"
#include "Nova/Core/Modules/Rendering/Layers/RenderLayer.h"
#include "Nova/Core/Events/Event.h"
#include "Nova/Core/Events/EventSource.h"

#include "ImGuiRenderer.h"

namespace Nova::ImGui
{
	/// <summary>
	/// Event called when ImGui is rendering a frame
	/// </summary>
	struct NovaAPI ImGuiRenderEvent : public Event
	{
		ImGuiRenderEvent(double deltaTime) :
			DeltaTime(deltaTime) {}

		double DeltaTime;
	};

	class NovaAPI ImGuiRenderLayer : public Rendering::RenderLayer
	{
	public:
		ImGuiRenderLayer();
		~ImGuiRenderLayer();

	// RenderLayer ----------
	public:
		virtual void BeginFrame(Ref<Rendering::GraphicsContext> currentContext, double deltaTime) override;
		virtual void EndFrame(Ref<Rendering::GraphicsContext> currentContext, double deltaTime) override;
		virtual int GetID() const override { return static_cast<int>(Rendering::DefaultRenderLayerIDs::ImGui); }

	// RenderLayer ----------

	private:
		void CreateRenderer();

	public:
		/// <summary>
		/// Invoked when this render layer is rendering a frame
		/// </summary>
		EventSource<ImGuiRenderEvent> OnBeginFrame;

	private:
		Exclusive<ImGuiRenderer> m_Renderer;
	};
}
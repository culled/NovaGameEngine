#pragma once

#include "RenderLayer.h"
#include "Nova/Core/Modules/Rendering/GraphicsContext.h"

namespace Nova::Rendering
{
	/// <summary>
	/// Holds a stack of RenderLayers
	/// </summary>
	class NovaAPI RenderLayerStack
	{
	public:
		/// <summary>
		/// Appends a RenderLayer to the end of the layer stack
		/// </summary>
		/// <param name="layer">The layer to add</param>
		void AppendLayer(const Ref<RenderLayer>& layer);

		/// <summary>
		/// Removes a RenderLayer from the layer stack
		/// </summary>
		/// <param name="layer">The layer to remove</param>
		void RemoveLayer(const Ref<RenderLayer>& layer);

		/// <summary>
		/// Begins a frame for each layer in the stack
		/// </summary>
		void BeginFrame(Ref<GraphicsContext> currentContext, double deltaTime);

		/// <summary>
		/// Ends a frame for each layer in the stack
		/// </summary>
		void EndFrame(Ref<GraphicsContext> currentContext, double deltaTime);

	private:
		/// <summary>
		/// The stack of RenderLayers
		/// </summary>
		List<Ref<RenderLayer>> m_Layers;
	};
}


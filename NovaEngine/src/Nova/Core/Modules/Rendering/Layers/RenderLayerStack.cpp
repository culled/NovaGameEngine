#include "RenderLayerStack.h"

namespace Nova::Rendering
{
	void RenderLayerStack::AppendLayer(const Ref<RenderLayer>& layer)
	{
		m_Layers.push_back(layer);
	}

	void RenderLayerStack::RemoveLayer(const Ref<RenderLayer>& layer)
	{
		auto it = std::find_if(m_Layers.begin(), m_Layers.end(), [layer](const Ref<RenderLayer>& otherLayer)
			{
				return layer.get() == otherLayer.get();
			});

		if (it != m_Layers.end())
		{
			m_Layers.erase(it);
		}
	}

	void RenderLayerStack::BeginFrame(Ref<GraphicsContext> currentContext, double deltaTime)
	{
		for (const auto& layer : m_Layers)
		{
			layer->BeginFrame(currentContext, deltaTime);
		}
	}

	void RenderLayerStack::EndFrame(Ref<GraphicsContext> currentContext, double deltaTime)
	{
		for (const auto& layer : m_Layers)
		{
			layer->EndFrame(currentContext, deltaTime);
		}
	}
}
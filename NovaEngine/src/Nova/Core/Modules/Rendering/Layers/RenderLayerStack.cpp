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

	void RenderLayerStack::BeginFrame(double deltaTime)
	{
		for (const auto& layer : m_Layers)
		{
			layer->BeginFrame(deltaTime);
		}
	}

	void RenderLayerStack::EndFrame()
	{
		for (const auto& layer : m_Layers)
		{
			layer->EndFrame();
		}
	}

	void RenderLayerStack::RenderContext(Ref<GraphicsContext> context)
	{
		for (const auto& layer : m_Layers)
		{
			layer->RenderContext(context);
		}
	}
}
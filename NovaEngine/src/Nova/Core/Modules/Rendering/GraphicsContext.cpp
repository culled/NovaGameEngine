#include "GraphicsContext.h"

namespace Nova::Rendering
{
	void GraphicsContext::RenderForLayer(int layerID)
	{
		auto it = m_LayerRenderingMap.find(layerID);

		// Emit the event for the layer (if there are any listeners)
		if (it != m_LayerRenderingMap.end())
		{
			(*it).second.EmitAnonymous();
		}
	}

	EventSource<GraphicsContext::LayerRenderingEventType>* GraphicsContext::GetLayerRenderEventSourcePtr(int layerID)
	{
		// Get a pointer to the EventSource for the layer ID. Creates one if it doesn't exist
		EventSource<LayerRenderingEventType>* sourcePtr = &m_LayerRenderingMap[layerID];

		return sourcePtr;
	}
}
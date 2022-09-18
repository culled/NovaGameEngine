#include "NodeTree.h"

#include "Nova/Core/App/App.h"

#include <algorithm>
#include <format>

namespace Nova
{
	TickEvent::TickEvent(double deltaTime) :
		DeltaTime(deltaTime)
	{}


	// RefCounted ----------
	void NodeTree::Init()
	{
		m_RootNode = MakeRef<Node>("Root");
		m_RootNode->SetTree(GetSelfRef<NodeTree>());
	}

	// RefCounted ----------

	void NodeTree::TickNode(const Ref<Node>& node, double deltaTime)
	{
		//node->Tick(deltaTime);

		if (node->m_Children.size() > 0)
		{
			List<Ref<Node>> childNodes(node->m_Children);
			//std::sort(childNodes.begin(), childNodes.end(), Node::TickPriorityComparator);
		
			for (const Ref<Node>& child : childNodes)
			{
				TickNode(child, deltaTime);
			}
		}
	}

	// TickListener ----------
	//void NodeTree::Tick(double deltaTime)
	//{
	//	OnPreTreeTick.EmitAnonymous(deltaTime);
	//
	//	TickNode(m_RootNode, deltaTime);
	//
	//	OnPostTreeTick.EmitAnonymous(deltaTime);
	//}

	// TickListener ----------

	void NodeTree::LayerRenderingFrame(int layerID)
	{
		auto it = m_LayerRenderingMap.find(layerID);

		// Emit the event for the layer (if there are any listeners)
		if (it != m_LayerRenderingMap.end())
		{
			(*it).second.EmitAnonymous();
		}
	}

	EventSource<NodeTree::LayerRenderingEventType>* NodeTree::GetLayerRenderEventSourcePtr(int layerID)
	{
		// Get a pointer to the EventSource for the layer ID. Creates one if it doesn't exist
		EventSource<LayerRenderingEventType>* sourcePtr = &m_LayerRenderingMap[layerID];

		return sourcePtr;
	}
}
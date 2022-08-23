#include "NodeTree.h"

#include "Nova/Core/App/App.h"

#include <algorithm>
#include <format>

namespace Nova
{
	// RefCounted ----------
	void NodeTree::Init()
	{
		m_RootNode = MakeRef<Node>("Root");
		m_RootNode->SetTree(GetSelfRef<NodeTree>());
	}

	// RefCounted ----------

	void NodeTree::TickNode(Ref<Node> node, double deltaTime)
	{
		node->Tick(deltaTime);

		if (node->m_Children.size() > 0)
		{
			List<Ref<Node>> childNodes(node->m_Children);
			std::sort(childNodes.begin(), childNodes.end(), Node::TickPriorityComparator);
		
			for (Ref<Node> child : childNodes)
			{
				TickNode(child, deltaTime);
			}
		}
	}

	// TickListener ----------
	void NodeTree::Tick(double deltaTime)
	{
		TickNode(m_RootNode, deltaTime);
	}

	// TickListener ----------
}
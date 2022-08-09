#include "NodeTree.h"

#include <algorithm>

namespace Nova
{
	NodeTree::NodeTree()
	{
		m_RootNode = MakeRef<Node>("Root", GetRef<NodeTree>());
	}

	void NodeTree::TickNode(Ref<Node> node, double deltaTime)
	{
		if (node->m_TickEnabled)
		{
			node->Tick(deltaTime);
		}

		List<Ref<Node>> childNodes(node->m_Children);
		std::sort(childNodes.begin(), childNodes.end(), Node::TickComparator);

		for (const auto& child : childNodes)
		{
			TickNode(child, deltaTime);
		}
	}

	void NodeTree::Tick(double deltaTime)
	{
		TickNode(m_RootNode, deltaTime);
	}
}
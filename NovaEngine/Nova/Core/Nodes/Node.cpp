#include "Node.h"
#include "NodeTree.h"

namespace Nova
{
	Node::Node(const string& name) :
		Node(name, nullptr)
	{}

	Node::Node(const string& name, Ref<NodeTree> tree) :
		m_Name(name), m_Tree(tree)
	{}

	void Node::Tick(double deltaTime) {}

	void Node::SetTree(Ref<NodeTree> tree)
	{
		m_Tree = tree;
	}

	void Node::ClearTree()
	{
		m_Tree = nullptr;
	}

	void Node::AddChild(Ref<Node> node)
	{
		node->SetTree(m_Tree);
		m_Children.push_back(node);
	}

	void Node::RemoveChild(Ref<Node> node)
	{
		auto it = std::find_if(m_Children.begin(), m_Children.end(), [node](const Ref<Node>& other) {
			return node.get() == other.get();
		});

		if (it != m_Children.end())
		{
			m_Children.erase(it);
			node->ClearTree();
		}
	}
}

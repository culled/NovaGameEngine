#include "Node.h"
#include "NodeTree.h"

namespace Nova
{
	Node::Node(const string& name) :
		m_Name(name)
	{}

	bool Node::TickPriorityComparator(const Ref<Node>& lhs, const Ref<Node>& rhs)
	{
		return lhs->GetTickOrder() > rhs->GetTickOrder();
	}

	void Node::SetIsActive(bool isActive)
	{
		m_IsActive = isActive;
	}

	bool Node::GetIsActiveInTree() const
	{
		const Node* current = this;
		bool isActive = true;

		// Go up our parent hierarchy until we reach the end or a parent is in-active
		while (isActive && current != nullptr)
		{
			isActive = current->m_IsActive;
			
			// If we should continue, make the parent (if one exists) the current node 
			if (isActive)
			{
				auto parent = current->m_Parent.lock();

				if (parent)
				{
					current = parent.get();
				}
				else
				{
					current = nullptr;
				}
			}
		}

		return isActive;
	}

	void Node::SetTree(WeakRef<NodeTree> tree)
	{
		m_Tree = tree;

		// Recursively set the tree for our children
		for (const auto& child : m_Children)
		{
			child->SetTree(tree);
		}
	}

	void Node::AddChild(Ref<Node> node)
	{
		m_Children.push_back(node);

		// Notify the new child that its parent changed
		node->SetParent(GetSelfWeakRef<Node>());
	}

	void Node::RemoveChild(Ref<Node> node)
	{
		auto it = std::find_if(m_Children.begin(), m_Children.end(), [node](const Ref<Node>& other) {
			return node.get() == other.get();
		});

		if (it != m_Children.end())
		{
			m_Children.erase(it);

			// Notify the new child that its parent changed
			(*it)->SetParent(WeakRef<Node>());
		}
	}

	void Node::SetParent(WeakRef<Node> node)
	{
		m_Parent = node;
		
		if (auto parent = m_Parent.lock())
		{
			// Set our tree to our parent's tree
			SetTree(parent->m_Tree);
		}
		else
		{
			// No parent, so no tree
			SetTree(WeakRef<NodeTree>());
		}
	}
}

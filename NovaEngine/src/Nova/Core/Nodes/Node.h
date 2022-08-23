#pragma once

#include "Nova/Core/Engine.h"

#include "Nova/Core/Events/TickListener.h"

namespace Nova
{
	class NodeTree;

	/// <summary>
	/// Base class for all nodes that live in a NodeTree
	/// </summary>
	class NovaAPI Node : public TickListener
	{
	public:
		/// <summary>
		/// Constructs a node with the given name and no tree owner
		/// </summary>
		/// <param name="name">The name for this node</param>
		Node(const string& name);

		virtual ~Node() = default;

	// TickListener ----------
	protected:
		virtual int GetTickOrder() const override { return m_TickOrder; }

		virtual void Tick(double deltaTime) override {}
	// TickListener ----------

	public:
		/// <summary>
		/// Comparator function to compare tick priorities between 2 nodes
		/// </summary>
		/// <param name="lhs">The first node</param>
		/// <param name="rhs">The second node</param>
		static bool TickPriorityComparator(const Ref<Node>& lhs, const Ref<Node>& rhs);
	
	public:
		/// <summary>
		/// Sets if this node is active
		/// </summary>
		/// <param name="isActive">The active state of this node</param>
		void SetIsActive(bool isActive);

		/// <summary>
		/// Gets if this node is active
		/// </summary>
		/// <returns>True if this node is active</returns>
		bool GetIsActive() const { return m_IsActive; }

		/// <summary>
		/// Gets if this node is active in the tree. Will return false if a parent of this node is in-active, even if this node itself is active
		/// </summary>
		/// <returns>True if this node is active in the tree</returns>
		bool GetIsActiveInTree() const;

		/// <summary>
		/// Sets the owning tree for this node
		/// </summary>
		/// <param name="tree">The owning tree for this node</param>
		void SetTree(WeakRef<NodeTree> tree);

		/// <summary>
		/// Adds a node as a child of this node
		/// </summary>
		/// <param name="node">The node to add as a child of this node</param>
		void AddChild(Ref<Node> node);

		/// <summary>
		/// Removes a node from this node's list of children
		/// </summary>
		/// <param name="node">The node to unparent</param>
		void RemoveChild(Ref<Node> node);

	private:
		void SetParent(WeakRef<Node> node);

	private:
		/// The name of this node
		string m_Name;

		/// The order of ticking for this node
		int m_TickOrder = 0;

		/// This node's active state
		bool m_IsActive = true;

		/// The parent of this node
		WeakRef<Node> m_Parent;

		/// The tree this node located in
		WeakRef<NodeTree> m_Tree;

		/// A list of this node's children
		List<Ref<Node>> m_Children;

		friend NodeTree;
	};
}
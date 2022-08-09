#pragma once

#include "Nova/Core/Engine.h"

namespace Nova
{
	class NodeTree;

	/// <summary>
	/// Base class for all nodes that live in a NodeTree
	/// </summary>
	NovaClass Node : public RefCounted<Node>
	{
	public:
		static bool TickComparator(const Ref<Node>& lhs, const Ref<Node>& rhs) { return lhs->m_TickPriority > rhs->m_TickPriority; }

	public:
		/// <summary>
		/// Constructs a node with the given name and no tree owner
		/// </summary>
		/// <param name="name">The name for this node</param>
		Node(const string& name);

		/// <summary>
		/// Constructs a node with the given name and tree owner
		/// </summary>
		/// <param name="name">The name for this node</param>
		/// <param name="tree">The tree that will own this node</param>
		Node(const string& name, Ref<NodeTree> tree);

		virtual ~Node() = default;

		/// <summary>
		/// Ticks this node
		/// </summary>
		/// <param name="deltaTime">The delta time to use for this tick</param>
		virtual void Tick(double deltaTime);

		/// <summary>
		/// Gets if this node can be ticked
		/// </summary>
		/// <returns>True if this node can be ticked</returns>
		bool GetTickEnabled() const { return m_TickEnabled; }

		/// <summary>
		/// Enables/disables ticking for this node
		/// </summary>
		/// <param name="tickEnabled">True if this node should respond to ticks</param>
		void SetTickEnabled(bool tickEnabled) { m_TickEnabled = tickEnabled; }

		/// <summary>
		/// Sets the owning tree for this node
		/// </summary>
		/// <param name="tree">The owning tree for this node</param>
		void SetTree(Ref<NodeTree> tree);

		/// <summary>
		/// Clears the owning tree of this node
		/// </summary>
		void ClearTree();

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
		string m_Name;

		bool m_TickEnabled = false;
		int m_TickPriority = 0;

		WeakRef<Node> m_Parent;
		List<Ref<Node>> m_Children;
		Ref<NodeTree> m_Tree;

		friend NodeTree;
	};
}
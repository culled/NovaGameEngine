#pragma once

#include "Nova/Core/Engine.h"

#include "Nova/Core/Events/TickListener.h"
#include "Node.h"

namespace Nova
{
	/// <summary>
	/// Class that manages a tree of nodes
	/// </summary>
	NovaClass NodeTree : public TickListener
	{
	public:
		virtual ~NodeTree() = default;

	public:
		/// <summary>
		/// Ticks the given node and all its children
		/// </summary>
		/// <param name="node">The node to tick (if it has ticking enabled)</param>
		/// <param name="deltaTime">The delta time to use for the tick</param>
		static void TickNode(Ref<Node> node, double deltaTime);

	// RefCounted ----------
	protected:
		virtual void Init() override;

	// RefCounted ----------

	// TickListener ----------
	protected:
		virtual int GetTickOrder() const { return 0; }
		/// <summary>
		/// Ticks this node tree
		/// </summary>
		/// <param name="deltaTime">The delta time to use for the tick</param>
		virtual void Tick(double deltaTime) override;

	// TickListener ----------

	private:
		// The root node of this tree
		Ref<Node> m_RootNode;
	};
}
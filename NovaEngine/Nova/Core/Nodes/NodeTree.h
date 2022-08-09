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
		/// <summary>
		/// Ticks the given node and all its children
		/// </summary>
		/// <param name="node">The node to tick (if it has ticking enabled)</param>
		/// <param name="deltaTime">The delta time to use for the tick</param>
		static void TickNode(Ref<Node> node, double deltaTime);

	public:
		NodeTree();

		virtual int GetTickOrder() const { return 0; }

	protected:
		/// <summary>
		/// Ticks this node tree
		/// </summary>
		/// <param name="deltaTime">The delta time to use for the tick</param>
		virtual void Tick(double deltaTime) override;

	private:
		Ref<Node> m_RootNode;
	};
}
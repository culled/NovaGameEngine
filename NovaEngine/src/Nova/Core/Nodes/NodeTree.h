#pragma once

#include "Nova/Core/EngineAPI.h"

#include "Nova/Core/Events/TickListener.h"
#include "Nova/Core/Events/Event.h"
#include "Nova/Core/Events/EventSource.h"
#include "Nova/Core/Types/Map.h"
#include "Nova/Core/Types/RefCounted.h"
#include "Node.h"

namespace Nova
{
	struct NovaAPI TickEvent : public Event
	{
		TickEvent(double deltaTime);

		const double DeltaTime;
	};

	/// <summary>
	/// Class that manages a tree of nodes
	/// </summary>
	class NovaAPI NodeTree : public RefCounted
	{
	public:
		using LayerRenderingEventType = Event;

	public:
		virtual ~NodeTree() = default;

	public:
		/// <summary>
		/// Ticks the given node and all its children
		/// </summary>
		/// <param name="node">The node to tick (if it has ticking enabled)</param>
		/// <param name="deltaTime">The delta time to use for the tick</param>
		static void TickNode(const Ref<Node>& node, double deltaTime);

	// RefCounted ----------
	protected:
		virtual void Init() override;

	// RefCounted ----------

	// TickListener ----------
	protected:
		//virtual int GetTickOrder() const { return 0; }
		//virtual void Tick(double deltaTime) override;

	// TickListener ----------

	public:
		/// <summary>
		/// Called when a frame for a render layer starts rendering
		/// </summary>
		/// <param name="layerID">The id of the render layer</param>
		virtual void LayerRenderingFrame(int layerID);

		/// <summary>
		/// Adds the given member function as a callback when the given layer is rendering for this NodeTree
		/// </summary>
		/// <param name="layerID">The ID of the layer</param>
		/// <param name="obj">A reference to the object with the function</param>
		/// <param name="func">The address of the member function</param>
		template<typename ClassType>
		void AddLayerRenderingListener(int layerID, const Ref<ClassType>& obj, EventFuncDelegate<ClassType, LayerRenderingEventType> func)
		{
			EventSource<LayerRenderingEventType>* sourcePtr = GetLayerRenderEventSourcePtr(layerID);

			sourcePtr->Connect(obj, func);
		}

		/// <summary>
		/// Removes the given member function from listening to when the given layer is rendering
		/// </summary>
		/// <param name="layerID">The ID of the layer</param>
		/// <param name="obj">A reference to the object with the function</param>
		/// <param name="func">The address to the function</param>
		template<typename ClassType>
		void RemoveLayerRenderingListener(int layerID, const Ref<ClassType>& obj, EventFuncDelegate<ClassType, LayerRenderingEventType> func)
		{
			auto it = m_LayerRenderingMap.find(layerID);

			if (it != m_LayerRenderingMap.end())
			{
				(*it).second.Disconnect(obj, func);
			}
		}

		/// <summary>
		/// Adds the given member function as a callback when the given layer is rendering for this NodeTree
		/// </summary>
		/// <param name="layerID">The ID of the layer</param>
		/// <param name="obj">A pointer to the object with the function</param>
		/// <param name="func">The address of the member function</param>
		template<typename ClassType>
		void AddLayerRenderingListener(int layerID, ClassType* objPtr, EventFuncDelegate<ClassType, LayerRenderingEventType> func)
		{
			EventSource<LayerRenderingEventType>* sourcePtr = GetLayerRenderEventSourcePtr(layerID);

			sourcePtr->Connect(objPtr, func);
		}

		/// <summary>
		/// Removes the given member function from listening to when the given layer is rendering
		/// </summary>
		/// <param name="layerID">The ID of the layer</param>
		/// <param name="obj">A pointer to the object with the function</param>
		/// <param name="func">The address to the function</param>
		template<typename ClassType>
		void RemoveLayerRenderingListener(int layerID, ClassType* objPtr, EventFuncDelegate<ClassType, LayerRenderingEventType> func)
		{
			auto it = m_LayerRenderingMap.find(layerID);

			if (it != m_LayerRenderingMap.end())
			{
				(*it).second.Disconnect(objPtr, func);
			}
		}

	private:
		EventSource<LayerRenderingEventType>* GetLayerRenderEventSourcePtr(int layerID);

	public:
		/// <summary>
		/// Invoked before all nodes are ticked
		/// </summary>
		EventSource<TickEvent> OnPreTreeTick;

		/// <summary>
		/// Invoked after all nodes have been ticked
		/// </summary>
		EventSource<TickEvent> OnPostTreeTick;

	private:
		/// The root node of this tree
		Ref<Node> m_RootNode;

		/// <summary>
		/// The map of event sources that trigger for when each RenderLayer renders a frame
		/// </summary>
		Map<int, EventSource<LayerRenderingEventType>> m_LayerRenderingMap;
	};
}
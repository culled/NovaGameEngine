#pragma once

#include "Nova/Core/Engine.h"
#include "Nova/Core/Events/Event.h"
#include "Nova/Core/Events/EventSource.h"

#include <functional>

namespace Nova::Rendering
{
	/// <summary>
	/// A container of graphics resources for anything that can be rendered to (windows, images, etc.)
	/// </summary>
	class NovaAPI GraphicsContext : public RefCounted
	{
	public:
		using LayerRenderingEventType = Event;

	public:
		virtual ~GraphicsContext() = default;

//	public:
//		/// <summary>
//		/// Gets the current context
//		/// </summary>
//		/// <returns>The current context</returns>
//		static Ref<GraphicsContext> GetCurrentContext() { return s_CurrentContext; }
//
//	protected:
//		/// <summary>
//		/// Sets the currently used GraphicsContext for GetCurrentContext()
//		/// </summary>
//		/// <param name="context">The current context</param>
//		static void SetCurrentContext(const Ref<GraphicsContext>& context) { s_CurrentContext = context; }
//
//	private:
//		/// <summary>
//		/// The currently active context (if any)
//		/// </summary>
//		static Ref<GraphicsContext> s_CurrentContext;

	public:
		/// <summary>
		/// Makes this graphics context current on the calling thread
		/// </summary>
		virtual void MakeCurrent() = 0;

		/// <summary>
		/// Releases this graphics context from being current on the calling thread
		/// </summary>
		virtual void ReleaseCurrent() = 0;

		/// <summary>
		/// Gets the implementation-specific function that loads extensions into this context
		/// </summary>
		/// <param name="procFunc">The function that returns pointers to extension functions</param>
		virtual void* GetExtensionsFunction() const = 0;

		/// <summary>
		/// Swaps the front and back buffers of this context
		/// </summary>
		virtual void SwapBuffers() = 0;

		/// <summary>
		/// Gets the size of this graphics context's framebuffer
		/// </summary>
		/// <returns>The size of this graphics context's framebuffer</returns>
		virtual Vector2i GetFramebufferSize() const = 0;

		/// <summary>
		/// Gets the platform-specific handle for the context
		/// </summary>
		/// <returns>The platform-specific handle for the context</returns>
		virtual void* GetPlatformHandle() const = 0;

		/// <summary>
		/// Gets if this context shares resources with another GraphicsContext
		/// </summary>
		/// <returns>True if this context shares resources</returns>
		virtual bool GetIsShared() const = 0;

	public:
		/// <summary>
		/// Adds the given member function as a callback when the given layer is rendering for this NodeTree
		/// </summary>
		/// <param name="layerID">The ID of the layer</param>
		/// <param name="obj">A reference to the object with the function</param>
		/// <param name="func">The address of the member function</param>
		template<typename ClassType>
		void AddLayerRenderingListener(int layerID, Ref<ClassType> obj, EventFuncDelegate<ClassType, LayerRenderingEventType> func)
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
		void RemoveLayerRenderingListener(int layerID, Ref<ClassType> obj, EventFuncDelegate<ClassType, LayerRenderingEventType> func)
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

		/// <summary>
		/// Called when this context is being rendered by a RenderLayer
		/// </summary>
		/// <param name="layerID">The ID of the layer that's rendering</param>
		void RenderForLayer(int layerID);

	private:
		/// <summary>
		/// Gets an EventSource pointer for a given layerID
		/// </summary>
		/// <param name="layerID">The layer ID</param>
		/// <returns>The EventSource for the layer ID</returns>
		EventSource<LayerRenderingEventType>* GetLayerRenderEventSourcePtr(int layerID);

	private:
		/// <summary>
		/// The map of event sources that trigger for when each RenderLayer renders a frame
		/// </summary>
		Map<int, EventSource<LayerRenderingEventType>> m_LayerRenderingMap;
	};
}
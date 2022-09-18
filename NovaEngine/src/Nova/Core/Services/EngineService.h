#pragma once

#include "Nova/Core/EngineAPI.h"

#include "Nova/Core/Events/TickListener.h"

namespace Nova
{
	class Engine;

	/// <summary>
	/// Base class for all engine services
	/// </summary>
	class NovaAPI EngineService
	{
	public:
		virtual ~EngineService() = default;

	protected:
		/// <summary>
		/// Creates a TickListener for the given function
		/// </summary>
		/// <param name="order">The order for the listener</param>
		/// <param name="func">The function to call once the listener ticks</param>
		/// <returns>The created TickListener</returns>
		template<typename ServiceClass>
		Ref<TickListener> CreateTickListener(int order, void(ServiceClass::* func)(double))
		{
			static_assert(std::is_base_of<EngineService, ServiceClass>::value, "The class must inherit from EngineService");

			return MakeRef<TickListener>(order, static_cast<ServiceClass*>(this), func);
		}

		/// <summary>
		/// Adds a TickListener to the Engine's main loop
		/// </summary>
		/// <param name="listener">The listener to add</param>
		void AddListenerToMainLoop(const Ref<TickListener>& listener) const;

		/// <summary>
		/// Removes a TickListener from the Engine's main loop
		/// </summary>
		/// <param name="listener">The listener to remove</param>
		void RemoveListenerFromMainLoop(const Ref<TickListener>& listener) const;
	};
}
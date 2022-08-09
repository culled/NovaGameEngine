#pragma once

#include "Nova/Core/Engine.h"

namespace Nova
{
	/// <summary>
	/// Base class for all classes that listen for ticks from the main loop
	/// </summary>
	NovaClass TickListener : public RefCounted<TickListener>
	{
	public:
		virtual ~TickListener() = default;

	protected:
		/// <summary>
		/// Ticks this listener
		/// </summary>
		/// <param name="deltaTime">The delta time to use for this tick</param>
		virtual void Tick(double deltaTime) = 0;

		/// <summary>
		/// Enables/disables this listener to/from responding to ticks
		/// </summary>
		/// <param name="listen">True if this listener should respond to ticks</param>
		void SetListenForTicks(bool listen) { m_ListenForTicks = listen; }

		/// <summary>
		/// Gets if this listener responds to ticks
		/// </summary>
		/// <returns>True if this listener responds to ticks</returns>
		bool GetListenForTicks() const { return m_ListenForTicks; }

		/// <summary>
		/// An abstract method that returns the order for this listener to be ticked at. Lower values = earlier tick
		/// </summary>
		/// <returns>An int representing this listener's tick order</returns>
		virtual int GetTickOrder() const = 0;

	private:
		bool m_ListenForTicks = false;

		friend class MainLoop;
	};
}
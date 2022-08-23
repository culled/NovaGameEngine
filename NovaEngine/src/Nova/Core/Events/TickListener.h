#pragma once

#include "Nova/Core/Engine.h"

namespace Nova
{
	/// <summary>
	/// Base class for all classes that can be ticked
	/// </summary>
	class NovaAPI TickListener : public RefCounted
	{
	public:
		virtual ~TickListener() = default;

	public:
		/// <summary>
		/// Calls Tick() if it is determined this listener should tick
		/// </summary>
		void NotifyTick(double deltaTime);
		
		/// <summary>
		/// An abstract method that returns the order for this listener to be ticked at. Lower values = earlier tick
		/// </summary>
		/// <returns>An int representing this listener's tick order</returns>
		virtual int GetTickOrder() const = 0;

		/// <summary>
		/// Called when this listener ticks
		/// </summary>
		/// <param name="deltaTime">The time since the last tick (in seconds)</param>
		virtual void Tick(double deltaTime) = 0;

		/// <summary>
		/// Sets if this listener should respond to ticks
		/// </summary>
		/// <param name="listen">If true, this listener will respond to ticks</param>
		void SetListenForTicks(bool listen) { m_ListenForTicks = listen; }

		/// <summary>
		/// Gets if this listener responds to ticks
		/// </summary>
		/// <returns>True if this listener responds to ticks</returns>
		bool GetListenForTicks() const { return m_ListenForTicks; }

		/// <summary>
		/// Sets the tick period for this listener. 0 = tick every frame
		/// </summary>
		/// <param name="period">The period between ticks (in seconds)</param>
		void SetTickPeriod(double period) { m_TickPeriod = period; }

		/// <summary>
		/// Gets the tick period for this listener
		/// </summary>
		/// <returns>The period between ticks (in seconds)</returns>
		double GetTickPeriod() const { return m_TickPeriod; }

	private:
		/// If true, then this listener will respond to ticks
		bool m_ListenForTicks = false;

		/// The period that this listener will respond to ticks (in seconds). 0 = every tick
		double m_TickPeriod = 0.0;

		/// The time since the last tick response (if ticks are enabled)
		double m_TimeSinceLastTick = 0.0;
	};
}
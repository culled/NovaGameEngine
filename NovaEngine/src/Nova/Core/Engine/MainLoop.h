#pragma once

#include "Nova/Core/EngineAPI.h"

#include "Nova/Core/Events/TickListener.h"
#include "Nova/Core/Types/DateTime.h"
#include "Nova/Core/Types/RefCounted.h"
#include "Nova/Core/Types/List.h"

namespace Nova
{
	/// <summary>
	/// Runs a loop for the engine, ticking all registered listeners
	/// </summary>
	class NovaAPI MainLoop
	{
	public:
		/// <summary>
		/// Creates a MainLoop with the given target tickrate
		/// </summary>
		/// <param name="targetTickrate">The target tickrate. Set to 0 to tick as fast as possible</param>
		MainLoop(int targetTickrate = 0);

		~MainLoop();

	private:
		/// <summary>
		/// Comparator for two TickListeners
		/// </summary>
		/// <param name="lhs">A TickListener</param>
		/// <param name="rhs">A TickListener</param>
		/// <returns>True if the left tick listener should come before the right listener</returns>
		static bool CompareTickListeners(const Ref<TickListener>& lhs, const Ref<TickListener>& rhs);

	private:
		/// <summary>
		/// The duration to set the current deltatime to at startup
		/// </summary>
		static const double s_StartupTickDuration;

	public:
		/// <summary>
		/// Adds a listener for ticks from this loop
		/// </summary>
		/// <param name="listener">The listener to add</param>
		void AddTickListener(const Ref<TickListener>& listener);

		/// <summary>
		/// Stops the listener from receiving ticks from this loop
		/// </summary>
		/// <param name="listener">The listener to remove</param>
		void RemoveTickListener(const Ref<TickListener>& listener);

		/// <summary>
		/// Starts this loop
		/// </summary>
		void Run();

		/// <summary>
		/// Stops this loop after the current tick
		/// </summary>
		void Stop();

		/// <summary>
		/// Sets the target tickrate for this loop. A value of 0 means tick as fast as possible
		/// </summary>
		/// <param name="ticksPerSecond">The target number of ticks per second, or 0 to tick as fast as possible</param>
		void SetTargetTickrate(int ticksPerSecond) { m_TargetTickrate = ticksPerSecond; }

		/// <summary>
		/// Gets the time between the current tick and the last tick
		/// </summary>
		/// <returns>The time between ticks (in seconds)</returns>
		double GetDeltaTime() const { return m_CurrentDeltaTime; }

	private:
		/// <summary>
		/// Sorts the list of tick listeners based on their order
		/// </summary>
		void SortTickListeners();

		/// <summary>
		/// Performs a tick
		/// </summary>
		void PerformTick();

		/// <summary>
		/// Waits for the next tick time
		/// </summary>
		void WaitForTargetTickrate();

	private:
		/// <summary>
		/// Gets the running state of this loop
		/// </summary>
		bool m_IsRunning = false;

		/// <summary>
		/// If true, the list of TickListeners needs to be re-sorted for the next tick
		/// </summary>
		bool m_IsListenerSortDirty = false;

		/// <summary>
		/// A list of all TickListeners listening to ticks from this loop
		/// </summary>
		List<Ref<TickListener>> m_TickListeners;

		/// <summary>
		/// The time listeners were last ticked
		/// </summary>
		DateTime m_LastTickTime;

		/// <summary>
		/// The current time between ticks
		/// </summary>
		double m_CurrentDeltaTime;

		/// <summary>
		/// The target ticks per second for this loop
		/// </summary>
		int m_TargetTickrate;
	};
}
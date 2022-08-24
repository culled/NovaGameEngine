#pragma once

#include "Nova/Core/Engine.h"

#include "Nova/Core/Events/TickListener.h"
#include "Nova/Core/Types/DateTime.h"

namespace Nova
{
	/// <summary>
	/// Base class for an application loop
	/// </summary>
	class NovaAPI MainLoop
	{
	public:
		virtual ~MainLoop() = default;

	private:
		/// <summary>
		/// Comparator for two TickListeners
		/// </summary>
		/// <param name="lhs">A TickListener</param>
		/// <param name="rhs">A TickListener</param>
		/// <returns>True if the left tick listener should come before the right listener</returns>
		static bool CompareTickListeners(const WeakRef<TickListener>& lhs, const WeakRef<TickListener>& rhs);

	public:
		/// <summary>
		/// Starts this loop
		/// </summary>
		virtual void Start();

		/// <summary>
		/// Stops this loop from continuing
		/// </summary>
		virtual void Stop();

		/// <summary>
		/// Adds a listener for ticks from this loop
		/// </summary>
		/// <param name="listener">The listener to add</param>
		void AddTickListener(Ref<TickListener> listener);

		/// <summary>
		/// Removes the listener from the list of listeners that will receive ticks
		/// </summary>
		/// <param name="listener">The listener to remove</param>
		void RemoveTickListener(Ref<TickListener> listener);

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
		/// Ticks all listeners in thier correct order
		/// </summary>
		void TickListeners();

	private:
		/// Gets the running state of this loop
		bool m_IsRunning = false;

		/// If true, listeners should be re-sorted before they're ticked
		bool m_IsListenerSortDirty = false;

		/// A list of all tick listeners
		List<WeakRef<TickListener>> m_TickListeners;

		/// The time listeners were last ticked
		TimeSpan m_LastTickTime;

		/// <summary>
		/// The current time between ticks
		/// </summary>
		double m_CurrentDeltaTime;
	};
}
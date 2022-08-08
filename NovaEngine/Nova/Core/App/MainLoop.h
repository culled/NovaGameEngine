#pragma once

#include "Nova/Core/Engine.h"
#include "Nova/Core/Events/TickListener.h"
#include "Nova/Core/Types/DateTime.h"
#include "Nova/Core/Types/List.h"

namespace Nova
{
	/// <summary>
	/// Base class for an application loop
	/// </summary>
	NovaClass MainLoop
	{
	public:
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

	private:
		/// <summary>
		/// Sorts the list of tick listeners based on their order
		/// </summary>
		void SortTickListeners();

		/// <summary>
		/// Ticks all listeners in thier correct order
		/// </summary>
		void TickListeners();

	protected:
		bool m_IsRunning = false;
		bool m_ListenersModifiedSinceLastTick = false;

		List<WeakRef<TickListener>> m_TickListeners;

		TimeSpan m_LastTickTime;
	};
}
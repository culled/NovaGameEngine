#pragma once

#include "Nova/Core/EngineAPI.h"
#include "Nova/Core/Types/RefCounted.h"

#include <functional>

namespace Nova
{
	/// <summary>
	/// A class that can be attached to a loop to invoke a callback function at specified intervals
	/// </summary>
	class NovaAPI TickListener : public RefCounted
	{
	public:
		/// <summary>
		/// Creates a TickListener for a RefCounted object member function
		/// </summary>
		/// <param name="tickOrder">The order for this listener</param>
		/// <param name="object">The object</param>
		/// <param name="tickFunc">The callback function</param>
		template<typename ClassType>
		TickListener(int tickOrder, const Ref<ClassType>& object, void(ClassType::* tickFunc)(double)) :
			m_TickOrder(tickOrder)
		{
			WeakRef<ClassType> objectWeakPtr(object);
			m_TickFunc = [objectWeakPtr, tickFunc](double deltaTime)
			{
				auto objectPtr = objectWeakPtr.lock();
				if (objectPtr)
					(objectPtr.get()->*tickFunc)(deltaTime);
			};
		}

		/// <summary>
		/// Creates a TickListener for a raw pointer member function
		/// </summary>
		/// <param name="tickOrder">The order for this listener</param>
		/// <param name="objectPtr">The object pointer</param>
		/// <param name="tickFunc">The callback function</param>
		template<typename ClassType>
		TickListener(int tickOrder, ClassType* objectPtr, void(ClassType::* tickFunc)(double)) : 
			m_TickOrder(tickOrder)
		{
			m_TickFunc = [objectPtr, tickFunc](double deltaTime)
			{
				if (objectPtr)
					(objectPtr->*tickFunc)(deltaTime);
			};
		}

	public:
		/// <summary>
		/// Invokes a tick if one is scheduled for this listener
		/// </summary>
		void NotifyTick(double deltaTime);
		
		/// <summary>
		/// An abstract method that returns the order for this listener to be ticked at. Lower values = earlier tick
		/// </summary>
		/// <returns>An int representing this listener's tick order</returns>
		int GetTickOrder() const { return m_TickOrder; }

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
		/// <summary>
		/// If true, then this listener will respond to ticks
		/// </summary>
		bool m_ListenForTicks = true;

		/// <summary>
		/// The period that this listener will respond to ticks (in seconds). 0 = every tick
		/// </summary>
		double m_TickPeriod = 0.0;

		/// <summary>
		/// The time since the last tick response (if ticks are enabled)
		/// </summary>
		double m_TimeSinceLastTick = 0.0;

		/// <summary>
		/// The tick order for this listener
		/// </summary>
		int m_TickOrder;

		/// <summary>
		/// The function this listener will call once a tick happens
		/// </summary>
		std::function<void(double)> m_TickFunc;
	};
}
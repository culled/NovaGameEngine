#pragma once

#include "Nova/Core/Engine.h"

namespace Nova
{
	/// <summary>
	/// Wrapper class that stores a counter for a type that is guaranteed to be thread-safe
	/// </summary>
	template<typename T>
	class NovaAPI AtomicCounter
	{
	public:
		/// <summary>
		/// Creates a counter for the given type
		/// </summary>
		/// <param name="value">The initial starting value. Defaults to 0</param>
		AtomicCounter(T value = static_cast<T>(0))
		{
			Set(value);
		}

		/// <summary>
		/// Sets the value of this counter
		/// </summary>
		/// <param name="value">The new value to store</param>
		void Set(T value)
		{
			m_Value.store(value, std::memory_order_release);
		}

		/// <summary>
		/// Gets the current value of this counter
		/// </summary>
		/// <returns>The current value of this counter</returns>
		T Get() const
		{
			return m_Value.load(std::memory_order_acquire);
		}

		/// <summary>
		/// Increments this counter by 1 and returns its new value
		/// </summary>
		/// <returns>The new value of the counter after it was incremented</returns>
		T Increment()
		{
			// Add one because fetch_add returns the value before the counter was changed
			return m_Value.fetch_add(1, std::memory_order_acq_rel) + 1;
		}

		/// <summary>
		/// Decrements this counter by 1 and returns its new value
		/// </summary>
		/// <returns>The new value of the counter after it was decremented</returns>
		T Decrement()
		{
			// Subtract one because the returned value is the value before the counter was changed
			return m_Value.fetch_sub(1, std::memory_order_acq_rel) - 1;
		}

		/// <summary>
		/// Adds the given value to the counter and returns its new value
		/// </summary>
		/// <param name="value">The value to add</param>
		/// <returns>The new value of the counter</returns>
		T Add(T value)
		{
			// Add the value because fetch_add returns the value before the counter was changed
			return m_Value.fetch_add(value, std::memory_order_acq_rel) + value;
		}

		/// <summary>
		/// Subtracts the given value from the counter and returns its new value
		/// </summary>
		/// <param name="value">The value to subtract</param>
		/// <returns>The new value of the counter</returns>
		T Subtract(T value)
		{
			// Subtract the value because fetch_sub returns the value before the counter was changed
			return m_Value.fetch_sub(value, std::memory_order_acq_rel) - value;
		}

		/// <summary>
		/// Adds the value to the counter and returns the new value of the counter
		/// </summary>
		/// <param name="addValue">The value to add</param>
		/// <returns>The new value of the counter, accounting for if a different thread changed its value while the add was trying to take place</returns>
		T CompareAndSwapAdd(T addValue)
		{
			T current;
			do
			{
				current = m_Value.load(std::memory_order_acquire);
			} while (!m_Value.compare_exchange_weak(current, current + addValue, std::memory_order_acq_rel));
			return current + addValue;
		}
	private:
		/// The saved value
		std::atomic<T> m_Value;
	};

	/// <summary>
	/// Serves as a thread-safe reference counter
	/// </summary>
	class NovaAPI AtomicRefCounter
	{
	public:
		/// <summary>
		/// Creates a thread-safe ref counter with the given starting amount of refs (defaults to 1)
		/// </summary>
		AtomicRefCounter(uint32_t startingRefs = 1)
		{
			m_Counter.Set(startingRefs);
		}

		/// <summary>
		/// Adds a ref to this counter
		/// </summary>
		/// <returns>The new number of refs this counter has</returns>
		uint32_t AddRef()
		{
			return m_Counter.Increment();
		}

		/// <summary>
		/// Removes a ref from this counter
		/// </summary>
		/// <returns>The new number of refs this counter has</returns>
		uint32_t RemoveRef()
		{
			return m_Counter.Decrement();
		}

		/// <summary>
		/// Removes a ref from this counter and returns if this counter has any remaining refs
		/// </summary>
		/// <returns>True if this counter has 0 refs</returns>
		bool UnRef()
		{
			return m_Counter.Decrement() == 0;
		}

		/// <summary>
		/// Gets the number of refs this counter has
		/// </summary>
		/// <returns>The number of refs this counter has</returns>
		uint32_t GetRefs() const
		{
			return m_Counter.Get();
		}
	private:
		/// The counter used for this reference counter
		AtomicCounter<uint32_t> m_Counter;
	};
}
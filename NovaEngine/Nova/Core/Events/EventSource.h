#pragma once

#include "Nova/Core/Engine.h"
#include "Nova/Core/Types/List.h"
#include "Event.h"

#include <functional>
#include <type_traits>
#include <memory>
#include <any>

namespace Nova
{
	template<typename T, typename = std::enable_if_t<std::is_base_of_v<Event, T>>>
	NovaClass EventBinding
	{
	public:
		using EventCallback = std::function<bool(Ref<T>)>;

		template<typename U>
		EventBinding(Ref<U> obj, void (U::* method)(Ref<T>))
		{
			WeakRef<U> weakRef = MakeWeakRef<U>(obj);
			m_Callback = [weakRef, method](Ref<T> e)
			{ 
				auto ref = weakRef.lock();

				if (ref)
				{
					std::invoke(method, ref, e);
					return true;
				}

				return false;
			};

			m_ObjHash = GetObjHash(obj);
			m_FuncHash = GetFuncHash(method);
		}

		bool Call(Ref<T> e)
		{
			return m_Callback(e);
		}

		template<typename U>
		bool Equals(Ref<U> obj, void (U::* method)(Ref<T>)) const
		{
			return GetObjHash(obj) == m_ObjHash && GetFuncHash(method) == m_FuncHash;
		}

		template<typename U>
		size_t GetObjHash(Ref<U> obj) const
		{
			std::hash<U*> h;
			return h(obj.get());
		}

		// TODO: Make this actually check if the method has the same address as the stored method
		template<typename U>
		size_t GetFuncHash(void (U::* method)(Ref<T>)) const
		{
			return 0;
		}

	private:
		size_t m_ObjHash;
		size_t m_FuncHash;
		EventCallback m_Callback;
	};

	/// <summary>
	/// Represents a source for an Event type
	/// </summary>
	template<typename T, typename = std::enable_if_t<std::is_base_of_v<Event, T>>>
	NovaClass EventSource
	{
	public:
		/// <summary>
		/// Connects the member function as a listener to this event source
		/// </summary>
		/// <param name="object">The object that the function belongs to</param>
		/// <param name="method">The address of the function</param>
		template<typename U, typename V, typename = std::enable_if_t<std::is_convertible_v<T&, V&>>>
		void Connect(Ref<U> object, void (U::* method)(Ref<V>))
		{
			// Create a binding with the object and member function
			EventBinding listener(object, method);
			m_Listeners.emplace(m_Listeners.begin(), listener);
		}

		/// <summary>
		/// Disconnects the member function from listening to this event source
		/// </summary>
		/// <param name="object">The object that the function belongs to</param>
		/// <param name="method">The address of the function</param>
		template<typename U, typename V, typename = std::enable_if_t<std::is_convertible_v<T&, V&>>>
		void Disconnect(Ref<U> object, void (U::* method)(Ref<V>))
		{
			auto it = std::find_if(m_Listeners.begin(), m_Listeners.end(), [object, method](const EventBinding<T>& binding)
				{
					return binding.Equals(object, method);
				});
			
			// If we're not at the end of the list, we found a match
			if (it != m_Listeners.end())
			{
				// If this disconnect occured due to the event emitting, make sure the next event won't be skipped
				if (m_CurrentEventIterator == it)
					m_WasCurrentListenerErased = true;
			
				auto newIt = m_Listeners.erase(it);
			
				if(m_WasCurrentListenerErased)
					m_CurrentEventIterator = newIt;
			}
		}

		/// <summary>
		/// Emits the given event to all listeners, or until its propagation is stopped
		/// </summary>
		/// <param name="e">The event to emit to listeners</param>
		void Emit(Ref<T> e)
		{
			m_CurrentEventIterator = m_Listeners.begin();
			
			while(m_CurrentEventIterator != m_Listeners.end())
			{
				m_WasCurrentListenerErased = false;

				EventBinding<T> binding = *m_CurrentEventIterator;

				if (!binding.Call(e))
				{
					// The current listener is no more, so erase it
					m_CurrentEventIterator = m_Listeners.erase(m_CurrentEventIterator);
					m_WasCurrentListenerErased = true;
				}
			
				// Stop propagating if the event requests it
				if (!e->Propagate)
					break;
			
				// If the current listener was erased, then the next one will take its spot and we shouldn't increment
				if (!m_WasCurrentListenerErased)
					m_CurrentEventIterator++;
			}
		}

	private:
		List<EventBinding<T>> m_Listeners;
		List<EventBinding<T>>::iterator m_CurrentEventIterator;
		bool m_WasCurrentListenerErased = false;
	};
}
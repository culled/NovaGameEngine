#pragma once

#include "Nova/Core/Engine.h"
#include "Event.h"

#include <functional>
#include <type_traits>
#include <memory>
#include <any>

namespace Nova
{
	/// <summary>
	/// Represents a weak binding to a member function pointer
	/// </summary>
	template<typename T, typename = std::enable_if_t<std::is_base_of_v<Event, T>>>
	NovaClass WeakEventBinding
	{
	public:
		using EventCallback = std::function<bool(Ref<T>)>;

		/// <summary>
		/// Creates a raw binding to a member function on the given object
		/// </summary>
		/// <param name="objPtr">The raw object to bind to</param>
		/// <param name="method">The function to bind to</param>
		template<typename U>
		WeakEventBinding(U* objPtr, void (U::* method)(Ref<T>))
		{
			m_Callback = [objPtr, method](Ref<T> e)
			{
				if (objPtr)
				{
					std::invoke(method, objPtr, e);
					return true;
				}

				return false;
			};

			// Store some info about the object and function for possible comparisons
			m_ObjHash = GetObjHash(objPtr);
			m_FuncHash = GetFuncHash(method);
		}

		/// <summary>
		/// Creates a weak binding to a member function on the given object
		/// </summary>
		/// <param name="obj">The object to bind to</param>
		/// <param name="method">The function to bind to</param>
		template<typename U>
		WeakEventBinding(Ref<U> obj, void (U::* method)(Ref<T>))
		{
			// Create a weak reference to the object and pass that to a lambda function so we can call the proper function while the object exists
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

			// Store some info about the object and function for possible comparisons
			m_ObjHash = GetObjHash(obj.get());
			m_FuncHash = GetFuncHash(method);
		}

		/// <summary>
		/// Attempts to call the bound function. Will return false if the call fails (i.e. the object was deleted)
		/// </summary>
		/// <param name="e">The event</param>
		/// <returns>True if the call was successful</returns>
		bool Call(Ref<T> e)
		{
			return m_Callback(e);
		}

		/// <summary>
		/// Checks if the given object and member function are the same ones that this binding is bound to
		/// </summary>
		/// <param name="obj">The object</param>
		/// <param name="method">The function on the object</param>
		/// <returns>True if this binding is bound to the function on the object</returns>
		template<typename U>
		bool Equals(U* objPtr, void (U::* method)(Ref<T>)) const
		{
			return GetObjHash(objPtr) == m_ObjHash && GetFuncHash(method) == m_FuncHash;
		}

		/// <summary>
		/// Checks if the given object and member function are the same ones that this binding is bound to
		/// </summary>
		/// <param name="obj">The object</param>
		/// <param name="method">The function on the object</param>
		/// <returns>True if this binding is bound to the function on the object</returns>
		template<typename U>
		bool Equals(Ref<U> obj, void (U::* method)(Ref<T>)) const
		{
			return GetObjHash(obj.get()) == m_ObjHash && GetFuncHash(method) == m_FuncHash;
		}

	private:
		/// <summary>
		/// Calculates a hash for a given object
		/// </summary>
		template<typename U>
		size_t GetObjHash(U* objPtr) const
		{
			std::hash<U*> h;
			return h(objPtr);
		}

		/// <summary>
		/// Calculates a hash for a given function
		/// </summary>	
		template<typename U>
		size_t GetFuncHash(void (U::* method)(Ref<T>)) const
		{
			// TODO: Make this actually check if the method has the same address as the stored method
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
		/// Connects the raw member function as a listener to this event source
		/// </summary>
		/// <param name="objectPtr">The raw object that the function belongs to</param>
		/// <param name="method">The address of the function</param>
		template<typename U, typename V, typename = std::enable_if_t<std::is_convertible_v<T&, V&>>>
		void Connect(U* objectPtr, void (U::* method)(Ref<V>))
		{
			// Create a binding with the object and member function
			AddBinding(WeakEventBinding(objectPtr, method));
		}

		/// <summary>
		/// Disconnects the raw member function from listening to this event source
		/// </summary>
		/// <param name="objectPtr">The raw object that the function belongs to</param>
		/// <param name="method">The address of the function</param>
		template<typename U, typename V, typename = std::enable_if_t<std::is_convertible_v<T&, V&>>>
		void Disconnect(U* objectPtr, void (U::* method)(Ref<V>))
		{
			auto it = std::find_if(m_Listeners.begin(), m_Listeners.end(), [objectPtr, method](const WeakEventBinding<T>& binding)
				{
					return binding.Equals(objectPtr, method);
				});

			TryRemoveBinding(it);
		}

		/// <summary>
		/// Connects the member function as a listener to this event source
		/// </summary>
		/// <param name="object">The object that the function belongs to</param>
		/// <param name="method">The address of the function</param>
		template<typename U, typename V, typename = std::enable_if_t<std::is_convertible_v<T&, V&>>>
		void Connect(Ref<U> object, void (U::* method)(Ref<V>))
		{
			// Create a binding with the object and member function
			AddBinding(WeakEventBinding(object, method));
		}

		/// <summary>
		/// Disconnects the member function from listening to this event source
		/// </summary>
		/// <param name="object">The object that the function belongs to</param>
		/// <param name="method">The address of the function</param>
		template<typename U, typename V, typename = std::enable_if_t<std::is_convertible_v<T&, V&>>>
		void Disconnect(Ref<U> object, void (U::* method)(Ref<V>))
		{
			auto it = std::find_if(m_Listeners.begin(), m_Listeners.end(), [object, method](const WeakEventBinding<T>& binding)
				{
					return binding.Equals(object, method);
				});
			
			TryRemoveBinding(it);
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

				WeakEventBinding<T> binding = *m_CurrentEventIterator;

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
		void AddBinding(WeakEventBinding<T> listener)
		{
			m_Listeners.emplace(m_Listeners.begin(), listener);
		}

		void TryRemoveBinding(List<WeakEventBinding<T>>::iterator it)
		{
			if (it == m_Listeners.end())
				return;

			// If this disconnect occured due to the event emitting, make sure the next event won't be skipped
			if (m_CurrentEventIterator == it)
				m_WasCurrentListenerErased = true;

			auto newIt = m_Listeners.erase(it);

			if (m_WasCurrentListenerErased)
				m_CurrentEventIterator = newIt;
		}

	private:
		List<WeakEventBinding<T>> m_Listeners;
		List<WeakEventBinding<T>>::iterator m_CurrentEventIterator;
		bool m_WasCurrentListenerErased = false;
	};
}
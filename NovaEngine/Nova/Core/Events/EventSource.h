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
		using EventCallback = std::function<void(T&)>;
		using EventValidCallback = std::function<bool()>;

		template<typename U>
		using EventFuncDelegate = void (U::*)(T&);

		/// <summary>
		/// Creates a raw binding to a member function on the given object
		/// </summary>
		/// <param name="objPtr">The raw object to bind to</param>
		/// <param name="method">The function to bind to</param>
		template<typename U>
		WeakEventBinding(U* objPtr, EventFuncDelegate<U> method)
		{
			m_Callback = [objPtr, method](T& e)
			{
				if (objPtr)
				{
					std::invoke(method, objPtr, e);
				}
			};

			m_ValidCallback = [objPtr]()
			{
				return objPtr != nullptr;
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
		WeakEventBinding(Ref<U> obj, EventFuncDelegate<U> method)
		{
			// Create a weak reference to the object and pass that to a lambda function so we can call the proper function while the object exists
			WeakRef<U> weakRef = MakeWeakRef<U>(obj);

			m_Callback = [weakRef, method](T& e)
			{ 
				auto ref = weakRef.lock();

				if (ref)
				{
					std::invoke(method, ref, e);
				}
			};

			m_ValidCallback = [weakRef]()
			{
				return weakRef.lock() != nullptr;
			};

			// Store some info about the object and function for possible comparisons
			m_ObjHash = GetObjHash(obj.get());
			m_FuncHash = GetFuncHash(method);
		}

		/// <summary>
		/// Attempts to call the bound function
		/// </summary>
		/// <param name="e">The event</param>
		void Call(T& e)
		{
			m_Callback(e);
		}

		bool IsValid() const
		{
			return m_ValidCallback();
		}

		/// <summary>
		/// Checks if the given object and member function are the same ones that this binding is bound to
		/// </summary>
		/// <param name="obj">The object</param>
		/// <param name="method">The function on the object</param>
		/// <returns>True if this binding is bound to the function on the object</returns>
		template<typename U>
		bool Equals(U* objPtr, EventFuncDelegate<U> method) const
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
		bool Equals(Ref<U> obj, EventFuncDelegate<U> method) const
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
		size_t GetFuncHash(EventFuncDelegate<U> method) const
		{
			// TODO: Make this actually check if the method has the same address as the stored method
			return 0;
		}

	private:
		size_t m_ObjHash;
		size_t m_FuncHash;
		EventCallback m_Callback;
		EventValidCallback m_ValidCallback;
	};

	/// <summary>
	/// Represents a source for an Event type
	/// </summary>
	template<typename T, typename = std::enable_if_t<std::is_base_of_v<Event, T>>>
	NovaClass EventSource
	{
	public:
		template<typename U, typename V, typename = std::enable_if_t<std::is_convertible_v<T&, V&>>>
		using EventFuncDelegate = void (U::*)(V&);

		/// <summary>
		/// Connects the raw member function as a listener to this event source
		/// </summary>
		/// <param name="objectPtr">The raw object that the function belongs to</param>
		/// <param name="method">The address of the function</param>
		template<typename U, typename V, typename = std::enable_if_t<std::is_convertible_v<T&, V&>>>
		void Connect(U* objectPtr, EventFuncDelegate<U, V> method)
		{
			// Create a binding with the object and member function
			AddBinding(WeakEventBinding<V>(objectPtr, method));
		}

		/// <summary>
		/// Disconnects the raw member function from listening to this event source
		/// </summary>
		/// <param name="objectPtr">The raw object that the function belongs to</param>
		/// <param name="method">The address of the function</param>
		template<typename U, typename V, typename = std::enable_if_t<std::is_convertible_v<T&, V&>>>
		void Disconnect(U* objectPtr, EventFuncDelegate<U, V> method)
		{
			auto it = std::find_if(m_Listeners.begin(), m_Listeners.end(), [objectPtr, method](const WeakEventBinding<T>& binding)
				{
					return binding.Equals(objectPtr, method);
				});

			RemoveBinding(it);
		}

		/// <summary>
		/// Connects the member function as a listener to this event source
		/// </summary>
		/// <param name="object">The object that the function belongs to</param>
		/// <param name="method">The address of the function</param>
		template<typename U, typename V, typename = std::enable_if_t<std::is_convertible_v<T&, V&>>>
		void Connect(Ref<U> object, EventFuncDelegate<U, V> method)
		{
			// Create a binding with the object and member function
			AddBinding(WeakEventBinding<V>(object, method));
		}

		/// <summary>
		/// Disconnects the member function from listening to this event source
		/// </summary>
		/// <param name="object">The object that the function belongs to</param>
		/// <param name="method">The address of the function</param>
		template<typename U, typename V, typename = std::enable_if_t<std::is_convertible_v<T&, V&>>>
		void Disconnect(Ref<U> object, EventFuncDelegate<U, V> method)
		{
			auto it = std::find_if(m_Listeners.begin(), m_Listeners.end(), [object, method](const WeakEventBinding<T>& binding)
				{
					return binding.Equals(object, method);
				});
			
			RemoveBinding(it);
		}

		/// <summary>
		/// Emits the given event to all listeners, or until its propagation is stopped
		/// </summary>
		/// <param name="e">The event to emit to listeners</param>
		void Emit(T& e)
		{
			// Make a copy of the list in-case it changes during the event emitting
			List<WeakEventBinding<T>> listeners(m_Listeners);

			for (WeakEventBinding<T> listener : listeners)
			{
				listener.Call(e);

				// Stop propagating if the event requests it
				if (!e.Propagate)
					break;
			}

			RemoveInvalidBindings();
		}

		/// <summary>
		/// Emits an event to all listeners, or until its propagation is stopped. Use this when you don't care about what the event returns
		/// </summary>
		/// <param name="args">The arguments to pass to the constructor of the event</param>
		template<typename ... Args>
		void EmitAnonymous(Args&& ... args)
		{
			T e(std::forward<Args>(args)...);
			Emit(e);
		}

	private:
		/// <summary>
		/// Adds a WeakEventBinding to the list of listeners
		/// </summary>
		/// <param name="listener">The binding to add</param>
		void AddBinding(WeakEventBinding<T> listener)
		{
			// Place at the front so recent listeners receive the event before later ones
			m_Listeners.emplace(m_Listeners.begin(), listener);
		}

		/// <summary>
		/// Removes the listener from the list of listeners
		/// </summary>
		/// <param name="it">The iterator of the listener to remove</param>
		void RemoveBinding(List<WeakEventBinding<T>>::iterator it)
		{
			// Don't remove if the iterator isn't in the list
			if (it == m_Listeners.end())
				return;

			m_Listeners.erase(it);
		}

		/// <summary>
		/// Removes invalid bindings from the list of listeners
		/// </summary>
		void RemoveInvalidBindings()
		{
			auto it = m_Listeners.begin();

			while (it != m_Listeners.end())
			{
				WeakEventBinding<T> binding = *it;

				if (!binding.IsValid())
				{
					it = m_Listeners.erase(it);
				}
				else
				{
					it++;
				}
			}
		}

	private:
		// The list of bindings to listeners
		List<WeakEventBinding<T>> m_Listeners;
	};
}
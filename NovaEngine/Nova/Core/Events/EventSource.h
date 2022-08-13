#pragma once

#include "Nova/Core/Engine.h"
#include "Event.h"

#include <functional>
#include <type_traits>
#include <memory>

namespace Nova
{
	template<typename EventType>
	using StaticEventFuncDelegate = std::function<void(EventType&)>;

	template<typename ClassType, typename EventType>
	using EventFuncDelegate = void (ClassType::*)(EventType&);

	/// <summary>
	/// Base event binding
	/// </summary>
	template<typename T, typename = std::enable_if_t<std::is_base_of_v<Event, T>>>
	class NovaAPI EventBinding
	{
	public:
		virtual ~EventBinding() = default;

	public:
		/// <summary>
		/// Gets if this binding is still valid
		/// </summary>
		/// <returns>True if this binding is valid</returns>
		virtual bool IsValid() const = 0;

		/// <summary>
		/// Attempts to call the bound function
		/// </summary>
		/// <param name="e">The event</param>
		virtual void Call(T& e) = 0;

		virtual bool Equals(EventBinding<T>* other) const = 0;
	};

	/// <summary>
	/// Event binding to a static function
	/// </summary>
	template<typename EventType, typename = std::enable_if_t<std::is_base_of_v<Event, EventType>>>
	class NovaAPI StaticEventBinding : public EventBinding<EventType>
	{
	public:
		/// <summary>
		/// Creates a binding to a static function
		/// </summary>
		/// <param name="func">The static function to bind to</param>
		StaticEventBinding(StaticEventFuncDelegate<EventType> func) :
			m_FuncDelegate(std::bind(func, std::placeholders::_1))
		{}

		~StaticEventBinding() = default;

	// EventBinding ----------
	public:
		virtual bool IsValid() const override { return true; }

		virtual void Call(EventType& e) override { m_FuncDelegate(e); }

		// BUG: Doesn't work properly
		virtual bool Equals(EventBinding<EventType>* other) const override
		{
			StaticEventBinding* otherStaticBinding = dynamic_cast<StaticEventBinding*>(other);

			if (!otherStaticBinding)
			{
				return false;
			}

			if (this->m_FuncDelegate.target_type() != otherStaticBinding->m_FuncDelegate.target_type())
			{
				return false;
			}

			auto thisTarget = this->m_FuncDelegate.target<void(EventType&)>();
			auto otherTarget = otherStaticBinding->m_FuncDelegate.target<void(EventType&)>();

			return thisTarget == otherTarget;
		}

	// EventBinding ----------

	private:
		//https://stackoverflow.com/questions/20833453/comparing-stdfunctions-for-equality
		//static size_t GetFuncAddress(StaticEventFuncDelegate<EventType> func) {
		//	typedef void(FuncType)(EventType&);
		//	FuncType** fnPointer = func.template target<FuncType*>();
		//	return (size_t)*fnPointer;
		//}

	private:
		StaticEventFuncDelegate<EventType> m_FuncDelegate;
	};

	/// <summary>
	/// Represents a binding to a member function on a raw pointer
	/// </summary>
	template<typename ClassType, typename EventType, typename = std::enable_if_t<std::is_base_of_v<Event, EventType>>>
	class NovaAPI PtrEventBinding : public EventBinding<EventType>
	{
	public:
		/// <summary>
		/// Creates a binding to a member function on the raw pointer to an object
		/// </summary>
		/// <param name="ptr">The object pointer to bind to</param>
		/// <param name="method">The function to bind to</param>
		PtrEventBinding(ClassType* ptr, EventFuncDelegate<ClassType, EventType> func) :
			m_Ptr(ptr), m_Func(func)
		{}

		~PtrEventBinding() = default;

	// EventBinding ----------
	public:
		virtual bool IsValid() const override { return m_Ptr != nullptr; }

		virtual void Call(EventType& e) override
		{
			if (IsValid())
			{
				(m_Ptr->*m_Func)(e);
			}
		}

		virtual bool Equals(EventBinding<EventType>* other) const override
		{
			PtrEventBinding* otherPtrBinding = dynamic_cast<PtrEventBinding*>(other);

			if (!otherPtrBinding)
			{
				return false;
			}

			return this->m_Ptr == otherPtrBinding->m_Ptr && this->m_Func == otherPtrBinding->m_Func;
		}

	// EventBinding ----------

	private:
		ClassType* m_Ptr;
		EventFuncDelegate<ClassType, EventType> m_Func;
	};

	/// <summary>
	/// Represents a binding to a member function on a Ref
	/// </summary>
	template<typename ClassType, typename EventType, typename = std::enable_if_t<std::is_base_of_v<Event, EventType>>>
	class NovaAPI RefEventBinding : public EventBinding<EventType>
	{
	public:
		/// <summary>
		/// Creates a binding to a member function on the given Ref
		/// </summary>
		/// <param name="ref">The Ref to bind to</param>
		/// <param name="method">The function to bind to</param>
		RefEventBinding(Ref<ClassType> ref, EventFuncDelegate<ClassType, EventType> func) :
			m_WeakRef(WeakRef<ClassType>(ref)), m_Func(func)
		{}

		~RefEventBinding() = default;

		// EventBinding ----------
		public:
			virtual bool IsValid() const override { return m_WeakRef.lock() != nullptr; }

			virtual void Call(EventType& e) override
			{
				if (IsValid())
				{
					auto ref = m_WeakRef.lock();
					(ref.get()->*m_Func)(e);
				}
			}

			virtual bool Equals(EventBinding<EventType>* other) const override
			{
				RefEventBinding* otherRefBinding = dynamic_cast<RefEventBinding*>(other);

				if (!otherRefBinding)
				{
					return false;
				}

				Ref<ClassType> selfPtr = this->m_WeakRef.lock();
				Ref<ClassType> otherPtr = otherRefBinding->m_WeakRef.lock();

				return selfPtr != nullptr && otherPtr != nullptr && selfPtr.get() == otherPtr.get() && this->m_Func == otherRefBinding->m_Func;
			}

			// EventBinding ----------

			private:
				WeakRef<ClassType> m_WeakRef;
				EventFuncDelegate<ClassType, EventType> m_Func;
	};

	/// <summary>
	/// Represents a source for an Event type
	/// </summary>
	template<typename EventType, typename = std::enable_if_t<std::is_base_of_v<Event, EventType>>>
	class NovaAPI EventSource
	{
	public:
		using BindingRef = std::shared_ptr<EventBinding<EventType>>;

		/// <summary>
		/// Connects the raw member function as a listener to this event source
		/// </summary>
		/// <param name="objectPtr">The raw object that the function belongs to</param>
		/// <param name="method">The address of the function</param>
		template<typename ClassType, typename DerivedEventType, typename = std::enable_if_t<std::is_convertible_v<EventType&, DerivedEventType&>>>
		void Connect(ClassType* objectPtr, EventFuncDelegate<ClassType, DerivedEventType> method)
		{
			BindingRef binding = MakeBindingRef<PtrEventBinding<ClassType, DerivedEventType>>(objectPtr, method);

			// Create a binding with the object and member function
			AddBinding(binding);
		}

		/// <summary>
		/// Disconnects the raw member function from listening to this event source
		/// </summary>
		/// <param name="objectPtr">The raw object that the function belongs to</param>
		/// <param name="method">The address of the function</param>
		template<typename ClassType, typename DerivedEventType, typename = std::enable_if_t<std::is_convertible_v<EventType&, DerivedEventType&>>>
		void Disconnect(ClassType* objectPtr, EventFuncDelegate<ClassType, DerivedEventType> method)
		{
			BindingRef binding = MakeBindingRef<PtrEventBinding<ClassType, DerivedEventType>>(objectPtr, method);

			RemoveBinding(binding);
		}

		/// <summary>
		/// Connects the member function as a listener to this event source
		/// </summary>
		/// <param name="object">The object that the function belongs to</param>
		/// <param name="method">The address of the function</param>
		template<typename ClassType, typename DerivedEventType, typename = std::enable_if_t<std::is_convertible_v<EventType&, DerivedEventType&>>>
		void Connect(Ref<ClassType> object, EventFuncDelegate<ClassType, DerivedEventType> method)
		{
			BindingRef binding = MakeBindingRef<RefEventBinding<ClassType, DerivedEventType>>(object, method);

			// Create a binding with the object and member function
			AddBinding(binding);
		}

		/// <summary>
		/// Disconnects the member function from listening to this event source
		/// </summary>
		/// <param name="object">The object that the function belongs to</param>
		/// <param name="method">The address of the function</param>
		template<typename ClassType, typename DerivedEventType, typename = std::enable_if_t<std::is_convertible_v<EventType&, DerivedEventType&>>>
		void Disconnect(Ref<ClassType> object, EventFuncDelegate<ClassType, DerivedEventType> method)
		{
			BindingRef binding = MakeBindingRef<RefEventBinding<ClassType, DerivedEventType>>(object, method);
			
			RemoveBinding(binding);
		}

//		/// <summary>
//		/// Connects the static function as a listener to this event source
//		/// </summary>
//		/// <param name="method">The address of the static function</param>
//		template<typename DerivedEventType, typename = std::enable_if_t<std::is_convertible_v<EventType&, DerivedEventType&>>>
//		void Connect(StaticEventFuncDelegate<DerivedEventType> method)
//		{
//			BindingRef binding = MakeBindingRef<StaticEventBinding<DerivedEventType>>(method);
//
//			// Create a binding with the object and member function
//			AddBinding(binding);
//		}
//
//		/// <summary>
//		/// Disconnects the static function from listening to this event source
//		/// </summary>
//		/// <param name="method">The address of the static function</param>
//		template<typename DerivedEventType, typename = std::enable_if_t<std::is_convertible_v<EventType&, DerivedEventType&>>>
//		void Disconnect(StaticEventFuncDelegate<DerivedEventType> method)
//		{
//			BindingRef binding = MakeBindingRef<StaticEventBinding<DerivedEventType>>(method);
//
//			RemoveBinding(binding);
//		}

		/// <summary>
		/// Emits the given event to all listeners, or until its propagation is stopped
		/// </summary>
		/// <param name="e">The event to emit to listeners</param>
		void Emit(EventType& e)
		{
			// Make a copy of the list in-case it changes during the event emitting
			List<BindingRef> listeners(m_Listeners);

			for (const auto& listener : listeners)
			{
				listener->Call(e);

				// Stop propagating if the event requests it
				if (!e.Propagate)
					break;
			}

			RemoveInvalidListeners();
		}

		/// <summary>
		/// Emits an event to all listeners, or until its propagation is stopped. Use this when you don't care about what the event returns
		/// </summary>
		/// <param name="args">The arguments to pass to the constructor of the event</param>
		template<typename ... Args>
		void EmitAnonymous(Args&& ... args)
		{
			EventType e(std::forward<Args>(args)...);
			Emit(e);
		}

		/// <summary>
		/// Disconnects all listeners
		/// </summary>
		void DisconnectAllListeners()
		{
			m_Listeners.clear();
		}

		/// <summary>
		/// Removes invalid listeners from the list of listeners
		/// </summary>
		void RemoveInvalidListeners()
		{
			auto it = m_Listeners.begin();

			while (it != m_Listeners.end())
			{
				if (!(*it)->IsValid())
				{
					it = m_Listeners.erase(it);
				}
				else
				{
					it++;
				}
			}
		}

		/// <summary>
		/// Gets the number of listeners. This will include invalid listeners until an event is emitted or RemoveInvalidListeners is called 
		/// </summary>
		/// <returns>The number of listeners</returns>
		size_t GetListenerCount() const { return m_Listeners.size(); }

	private:
		/// <summary>
		/// Adds a WeakEventBinding to the list of listeners
		/// </summary>
		/// <param name="listener">The binding to add</param>
		void AddBinding(const BindingRef& listener)
		{
			// Place at the front so recent listeners receive the event before later ones
			m_Listeners.emplace(m_Listeners.begin(), listener);
		}

		/// <summary>
		/// Removes the listener from the list of listeners
		/// </summary>
		/// <param name="it">The iterator of the listener to remove</param>
		void RemoveBinding(const BindingRef& listener)
		{
			auto it = std::find_if(m_Listeners.begin(), m_Listeners.end(), [listener](const BindingRef& otherBinding)
			{
				return otherBinding->Equals(listener.get());
			});

			// Don't remove if the iterator isn't in the list
			if (it == m_Listeners.end())
				return;

			m_Listeners.erase(it);
		}

		/// <summary>
		/// Creates a BindingRef
		/// </summary>
		/// <param name="...args">The arguments to pass to the BindingRef's constructor</param>
		/// <returns>A new BindingRef</returns>
		template<typename BindingType, typename ... Args>
		BindingRef MakeBindingRef(Args&& ... args)
		{
			return std::make_shared<BindingType>(std::forward<Args>(args)...);
		}

	private:
		/// The list of bindings to listeners
		List<BindingRef> m_Listeners;
	};
}
#pragma once

#include "Nova/Core/Engine.h"

#include <type_traits>
#include <memory>

namespace Nova
{
	/// <summary>
	/// A sharable reference to an object
	/// </summary>
	template <typename T>
	using Ref = std::shared_ptr<T>;

	/// <summary>
	/// A weak (not counted) reference to a managed object
	/// </summary>
	template <typename T>
	using WeakRef = std::weak_ptr<T>;

	/// <summary>
	/// An exclusive reference to an object
	/// </summary>
	template<typename T>
	using Exclusive = std::unique_ptr<T>;

	/// <summary>
	/// Creates a managed object that cannot be shared
	/// </summary>
	/// <param name="...args">The arguments to pass to the class's constructor</param>
	/// <returns>The managed object</returns>
	template<typename T, typename ... Args>
	inline Exclusive<T> MakeExclusive(Args&& ...args) { return std::make_unique<T>(std::forward<Args>(args)...); }

	/// <summary>
	/// Base class for all reference-counted objects
	/// </summary>
	NovaClass RefCounted : public std::enable_shared_from_this<RefCounted>
	{
	public:
		virtual ~RefCounted() = default;

	public:
		/// <summary>
		/// Called when the RefCounted object has been constructed. It's safe to use GetSelfRef/GetSelfWeakRef now
		/// </summary>
		virtual void Init() {}

	protected:
		/// <summary>
		/// Gets a reference to this object. NOTE: cannot be used in the the object's constructor
		/// </summary>
		/// <returns>A reference to this object</returns>
		//template<typename T, typename = std::enable_if_t<std::is_base_of_v<RefCounted, T>>>
		template<typename T>
		Ref<T> GetSelfRef()
		{
			return std::dynamic_pointer_cast<T>(this->shared_from_this());
		}

		/// <summary>
		/// Gets a new weak reference to this object. NOTE: cannot be used in the the object's constructor
		/// </summary>
		/// <returns>A new weak reference to this object</returns>
		//template<typename T, typename = std::enable_if_t<std::is_base_of_v<RefCounted, T>>>
		template<typename T>
		WeakRef<T> GetSelfWeakRef()
		{
			return GetSelfRef<T>();
		}
	};

	/// <summary>
	/// Creates a RefCounted object
	/// </summary>
	/// <param name="...args">The arguments to pass to the class's constructor</param>
	/// <returns>A reference to the created object</returns>
	template<typename T, typename ... Args>
	inline Ref<T> MakeRef(Args&& ...args)
	{
		static_assert(std::is_base_of<RefCounted, T>::value, "Only RefCounted objects can be created");

		Ref<T> obj = std::make_shared<T>(std::forward<Args>(args)...);
		obj->Init();
		return obj;
	}

	/// <summary>
	/// Creates a weak (not counted) reference from a Ref
	/// </summary>
	/// <param name="ref">The object reference</param>
	/// <returns>A weak reference to the object</returns>
	template<typename T>
	inline WeakRef<T> MakeWeakRef(Ref<T> ref)
	{
		static_assert(std::is_base_of<RefCounted, T>::value, "Only RefCounted objects can be created");

		return std::weak_ptr<T>(ref);
	}

	/// <summary>
	/// Creates an empty weak reference
	/// </summary>
	/// <returns>An empty weak reference</returns>
	template<typename T>
	inline WeakRef<T> MakeWeakRef() { return std::weak_ptr<T>(); }
}
#pragma once

#include "Nova/Core/Engine.h"

#include <type_traits>
#include <memory>

namespace Nova
{
	/// <summary>
	/// A sharable reference to an object
	/// </summary>
	template<typename T>
	using Ref = std::shared_ptr<T>;

	/// <summary>
	/// A weak reference to a managed object
	/// </summary>
	template<typename T>
	using WeakRef = std::weak_ptr<T>;

	/// <summary>
	/// Base class for all reference-counted objects
	/// </summary>
	NovaClass RefCounted : public std::enable_shared_from_this<RefCounted>
	{
	public:
		virtual ~RefCounted() = default;

		virtual void Init() {}

		/// <summary>
		/// Gets a new reference for this object
		/// </summary>
		/// <returns>A new reference for this object</returns>
		template<typename T>
		Ref<T> GetRef()
		{
			return std::dynamic_pointer_cast<T>(this->shared_from_this());
		}

		/// <summary>
		/// Gets a new weak reference for this object
		/// </summary>
		/// <returns>A new weak reference for this object</returns>
		template<typename T>
		WeakRef<T> GetWeakRef()
		{
			return std::dynamic_pointer_cast<T>(this->shared_from_this());
		}
	};

	/// <summary>
	/// Creates a managed object that can be shared
	/// </summary>
	/// <param name="...args">The arguments to pass to the class's constructor</param>
	/// <returns>A reference to the created object</returns>
	template<typename T, typename ... Args>
	inline Ref<T> MakeRef(Args&& ...args)
	{
		Ref<T> obj = std::make_shared<T>(std::forward<Args>(args)...);

		// Only enabled if T is derived from RefCounted
		if constexpr (std::is_base_of_v<RefCounted, T>)
		{
			obj.get()->Init();
		}

		return obj;
	}

	/// <summary>
	/// Creates a weak reference from a reference
	/// </summary>
	/// <param name="ref">The reference to make the weak reference from</param>
	/// <returns>A weak reference to the original reference</returns>
	template<typename T>
	inline WeakRef<T> MakeWeakRef(Ref<T> ref) { return std::weak_ptr<T>(ref); }

	/// <summary>
	/// An exclusive reference to an object
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template<typename T>
	using Exclusive = std::unique_ptr<T>;

	/// <summary>
	/// Creates a managed object that cannot be easily shared
	/// </summary>
	/// <param name="...args">The arguments to pass to the class's constructor</param>
	/// <returns>The managed object</returns>
	template<typename T, typename ... Args>
	inline Exclusive<T> MakeExclusive(Args&& ...args) { return std::make_unique<T>(std::forward<Args>(args)...); }
}
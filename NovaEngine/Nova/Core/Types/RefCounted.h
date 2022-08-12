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
	class NovaAPI RefCounted : public std::enable_shared_from_this<RefCounted>
	{
	public:
		virtual ~RefCounted() = default;

	public:
		/// <summary>
		/// Creates a RefCounted object
		/// </summary>
		/// <param name="...args">The arguments to pass to the class's constructor</param>
		/// <returns>A reference to the created object</returns>
		template <typename T, typename ... Args>
		static Ref<T> Create(Args&& ... args)
		{
			static_assert(std::is_base_of<RefCounted, T>::value, "Only RefCounted objects can be created");

			// Create the object
			Ref<T> obj = std::make_shared<T>(std::forward<Args>(args)...);

			// Cast to our RefCounted type so we can call the protected Init function
			Ref<RefCounted> objRef = static_pointer_cast<RefCounted>(obj);
			objRef->Init();

			return obj;
		}

	protected:
		/// <summary>
		/// Called when the RefCounted object has been constructed. It's safe to use GetSelfRef/GetSelfWeakRef now
		/// </summary>
		virtual void Init() {}

	protected:
		/// <summary>
		/// Gets a reference to this object. NOTE: cannot be used in the the object's constructor
		/// </summary>
		/// <returns>A reference to this object</returns>
		template<typename T>
		Ref<T> GetSelfRef()
		{
			return std::dynamic_pointer_cast<T>(this->shared_from_this());
		}

		/// <summary>
		/// Gets a new weak reference to this object. NOTE: cannot be used in the the object's constructor
		/// </summary>
		/// <returns>A new weak reference to this object</returns>
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
	Ref<T> MakeRef(Args&& ... args)
	{
		static_assert(std::is_base_of<RefCounted, T>::value, "Only RefCounted objects can be created");

		return RefCounted::Create<T>(std::forward<Args>(args)...);
	}
}
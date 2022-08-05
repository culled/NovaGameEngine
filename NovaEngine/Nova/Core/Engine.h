#pragma once

#include <memory>

// Define platform support
#ifdef _WIN32 // Win32 always available for Windows platforms
#ifdef _WIN64
#define PLATFORM_WINDOWS
#include "Nova/Platform/Windows/Core/Engine_Windows.h"
#else
#error "Windows x86 is not supported"
#endif
#else
#error "Windows x64 is the only supported platform so far"
#endif

#define NovaClass class NOVA_API
#define NovaStruct struct NOVA_API

namespace Nova
{
	/// <summary>
	/// A reference to an object
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template<typename T>
	using Ref = std::shared_ptr<T>;

	/// <summary>
	/// Creates a reference counted object on the heap
	/// </summary>
	/// <param name="...args">The arguments to pass to the class's constructor</param>
	/// <returns>A reference to the created object</returns>
	template<typename T, typename ... Args>
	inline Ref<T> MakeRef(Args&& ...args) { return std::make_shared<T>(std::forward<Args>(args)...); }

	/// <summary>
	/// A managed pointer reference to an object that cannot be shared freely
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template<typename T>
	using Scope = std::unique_ptr<T>;

	/// <summary>
	/// Creates a managed object that cannot be shared freely
	/// </summary>
	/// <param name="...args">The arguments to pass to the class's constructor</param>
	/// <returns>The managed object</returns>
	template<typename T, typename ... Args>
	inline Scope<T> MakeScope(Args&& ...args) { return std::make_unique<T>(std::forward<Args>(args)...); }
}
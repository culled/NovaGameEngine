#pragma once

#include "Nova/Core/Engine.h"

#include <functional>

namespace Nova::Rendering
{
	/// <summary>
	/// Base class for a graphics context
	/// </summary>
	class NovaAPI GraphicsContext : public RefCounted
	{
	public:
		using MakeCurrentDelegate = std::function<void()>;

	public:
		GraphicsContext() = default;
		virtual ~GraphicsContext() = default;

	public:
		/// <summary>
		/// Makes this graphics context current
		/// </summary>
		virtual void MakeCurrent() = 0;

		/// <summary>
		/// For windowing systems that own a graphics context, this allows them to set a callback so when this
		/// graphics context is made current, it will use this callback instead
		/// </summary>
		/// <param name="func">The callback to use when this context is requested to be made current</param>
		virtual void SetMakeCurrentCallback(MakeCurrentDelegate func) = 0;

		/// <summary>
		/// Loads extensions for this graphics context
		/// </summary>
		/// <param name="procFunc">The function that returns pointers to extension functions</param>
		virtual void LoadExtensions(void* procFunc) = 0;
	};
}
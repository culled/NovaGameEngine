#pragma once

#include "Nova/Core/EngineAPI.h"
#include "Nova/Core/Types/RefCounted.h"
#include "Window.h"

namespace Nova::Windowing
{
	/// <summary>
	/// Types of windowing APIs
	/// </summary>
	enum class WindowingAPI
	{
		SDL = 0
	};

	/// <summary>
	/// Represents a windowing api that actually manages windows on the current platform
	/// </summary>
	class NovaAPI WindowingBackend
	{
	public:
		virtual ~WindowingBackend() = default;

	public:
		/// <summary>
		/// Creates a window with the given parameters
		/// </summary>
		/// <param name="params">The parameters to create the window with</param>
		/// <returns>The created window</returns>
		virtual Ref<Window> CreateWindow(const WindowCreateParams& params) = 0;

		/// <summary>
		/// Ticks this WindowingBackend
		/// </summary>
		/// <param name="deltaTime">The time between ticks</param>
		virtual void Tick(double deltaTime) = 0;
	};
}
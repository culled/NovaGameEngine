#pragma once

#include "Nova/Core/Engine.h"
#include "Window.h"
#include "Monitor.h"

namespace Nova::Windowing
{
	/// <summary>
	/// Available windowing APIs
	/// </summary>
	enum class WindowingAPI
	{
		GLFW
	};

	/// <summary>
	/// Base class for all windowing backends.
	/// A backend gets created by the WindowingModule and handles the implementation details of managing windows
	/// </summary>
	class NovaAPI WindowingBackend : public RefCounted
	{
	public:
		virtual ~WindowingBackend() = default;

	public:
		/// <summary>
		/// Gets the windowing API this backend uses
		/// </summary>
		/// <returns>This backend's windowing API</returns>
		virtual WindowingAPI GetAPI() const = 0;

		/// <summary>
		/// Creates a window
		/// </summary>
		/// <param name="createParams">The parameters for the window</param>
		/// <returns>The created window</returns>
		virtual Ref<Window> CreateWindow(const WindowCreateParams& createParams) = 0;

		/// <summary>
		/// Ticks this backend
		/// </summary>
		/// <param name="deltaTime">The time between this tick and the last (in seconds)</param>
		virtual void Tick(double deltaTime) = 0;

		/// <summary>
		/// Gets a list of monitors attached to this system
		/// </summary>
		/// <returns>A list of monitors attached to this system</returns>
		virtual List<Ref<Monitor>> GetMonitors() const = 0;
	};
}
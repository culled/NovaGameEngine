#pragma once

#include "Nova/Core/Engine.h"
#include "Window.h"

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
		/// Creates a window and adds it to this windowing module's list of windows
		/// </summary>
		/// <param name="width">The initial width of the window</param>
		/// <param name="height">The initial height of the window</param>
		/// <param name="title">The title of the window</param>
		/// <returns>The created window</returns>
		virtual Ref<Window> CreateAndAddWindow(const WindowCreateParams& createParams) = 0;

		/// <summary>
		/// Removes a window from this backend
		/// </summary>
		/// <param name="window">The window that closed</param>
		virtual void RemoveWindow(Ref<Window> window) = 0;

		/// <summary>
		/// Sets the given window as the "main window" for this application
		/// </summary>
		/// <param name="window">The window to set as the main window</param>
		virtual void SetMainWindow(Ref<Window> window) = 0;

		/// <summary>
		/// Gets the main window for this application (if one has been assigned/created)
		/// </summary>
		/// <returns>The main window, or nullptr if no window has been created/assigned</returns>
		virtual Ref<Window> GetMainWindow() const = 0;

		/// <summary>
		/// Ticks this backend
		/// </summary>
		/// <param name="deltaTime">The time between this tick and the last (in seconds)</param>
		virtual void Tick(double deltaTime) = 0;
	};
}
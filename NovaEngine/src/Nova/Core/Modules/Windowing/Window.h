#pragma once

#include "Nova/Core/Engine.h"

#include "Nova/Core/Events/EventSource.h"
#include "Nova/Core/Modules/Rendering/GraphicsContext.h"

namespace Nova::Windowing
{
	/// <summary>
	/// Event for when a window is trying to close
	/// </summary>
	struct NovaAPI WindowClosingEvent : public Event
	{
		WindowClosingEvent();
		~WindowClosingEvent() = default;

		// If set to false, the window will not close
		bool ShouldClose;
	};

	/// <summary>
	/// Event for when a window is resized
	/// </summary>
	struct NovaAPI WindowResizedEvent : public Event
	{
		WindowResizedEvent(Vector2i newSize);
		~WindowResizedEvent() = default;

		/// The new size of the window
		Vector2i NewSize;
	};

	/// <summary>
	/// Struct with info for creating windows
	/// </summary>
	/// <param name="size">The initial size for the window</param>
	/// <param name="title">The title for the window</param>
	struct NovaAPI WindowCreateParams
	{
		WindowCreateParams(const Vector2i& size, const string& title);
		~WindowCreateParams() = default;

		Vector2i InitialSize;
		string Title;
		bool VSync;
		Vector2i InitialPosition;
		Ref<Rendering::GraphicsContext> ShareGfxContext;
	};

	/// <summary>
	/// Represents a window on a user's desktop
	/// </summary>
	class NovaAPI Window : public RefCounted
	{
	public:
		virtual ~Window() = default;

	public:
		/// <summary>
		/// Sets the size of this window
		/// </summary>
		/// <returns>The desired size for this window</returns> 
		virtual void SetSize(const Vector2i& size) = 0;

		/// <summary>
		/// Gets the current size of this window
		/// </summary>
		/// <returns>The current size of this window</returns> 
		virtual Vector2i GetSize() const = 0;

		/// <summary>
		/// Sets the position of this window
		/// </summary>
		/// <param name="position">The desired position for this window</param>
		virtual void SetPosition(const Vector2i& position) = 0;

		/// <summary>
		/// Gets the current position of this window
		/// </summary>
		/// <returns>The current position of this window</returns>
		virtual Vector2i GetPosition() const = 0;

		/// <summary>
		/// Sets the title of this window
		/// </summary>
		/// <param name="title">The new title for this window</param>
		virtual void SetTitle(const string& title) = 0;

		/// <summary>
		/// Gets the current title of this window
		/// </summary>
		/// <returns>The current title of this window</returns> 
		virtual string GetTitle() const = 0;

		/// <summary>
		/// Enables/disables vsync for this window
		/// </summary>
		/// <param name="enabled">If true, vsync will be enabled</param>
		virtual void SetVSync(bool enabled) = 0;

		/// <summary>
		/// Gets if vsync is enabled for this window
		/// </summary>
		/// <returns>True if vsync is enabled for this window</returns>
		virtual bool GetVSync() const = 0;

		/// <summary>
		/// Shows this window if it is hidden
		/// </summary>
		virtual void Show() = 0;

		/// <summary>
		/// Hides this window if it is shown
		/// </summary>
		virtual void Hide() = 0;

		/// <summary>
		/// Gets if this window is currently hidden
		/// </summary>
		/// <returns>True if this window is hidden</returns>
		virtual bool GetIsHidden() const = 0;

		/// <summary>
		/// Focuses this window
		/// </summary>
		virtual void Focus() = 0;

		/// <summary>
		/// Gets if this window is currently focused
		/// </summary>
		/// <returns>True if this window is focused</returns>
		virtual bool GetIsFocused() const = 0;

		/// <summary>
		/// Gets if this window is minimized
		/// </summary>
		/// <returns>True if this window is minimized</returns>
		virtual bool GetIsMinimized() const = 0;
		
		/// <summary>
		/// Sets this window's opacity (if it is supported)
		/// </summary>
		/// <param name="opacity">The new opacity</param>
		virtual void SetOpacity(double opacity) = 0;

		/// <summary>
		/// Gets this window's opacity
		/// </summary>
		/// <returns>This window's opacity</returns>
		virtual double GetOpacity() const = 0;

		/// <summary>
		/// Gets this window's graphics context
		/// </summary>
		/// <returns>This window's graphics context</returns>
		virtual Ref<Rendering::GraphicsContext> GetGraphicsContext() const = 0;

		/// <summary>
		/// Gets the backend-specific window handle
		/// </summary>
		/// <returns>The backend-specific window handle</returns>
		virtual void* GetBackendWindowHandle() const = 0;

		/// <summary>
		/// Gets the platform-specific window handle
		/// </summary>
		/// <returns>The platform-specific window handle</returns>
		virtual void* GetPlatformWindowHandle() const = 0;

		/// <summary>
		/// Tells this window to close
		/// </summary>
		/// <param name="forceClosed">If true, this window will still close even if a listener in the OnClosing event sets ShouldClose to false</param>
		virtual void Close(bool forceClosed = false) = 0;

	protected:
		/// <summary>
		/// Sets if this window is the main window of the application
		/// </summary>
		/// <param name="isMain">True if this window should act as the main window for the application</param>
		virtual void SetMainWindow(bool isMain) = 0;

	public:
		/// <summary>
		/// Invoked when this window is trying to close
		/// </summary>
		EventSource<WindowClosingEvent> OnClosing;

		/// <summary>
		/// Invoked when this window has been closed
		/// </summary>
		EventSource<Event> OnClosed;

		/// <summary>
		/// Invoked when this window is resized
		/// </summary>
		EventSource<WindowResizedEvent> OnResized;

	protected:
		friend class WindowingModule;
	};
}
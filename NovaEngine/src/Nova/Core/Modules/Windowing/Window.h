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
		// If set to false, the window will not close
		bool ShouldClose = true;
	};

	/// <summary>
	/// Event for when a window is resized
	/// </summary>
	struct NovaAPI WindowResizedEvent : public Event
	{
		WindowResizedEvent(uint32_t newWidth, uint32_t newHeight) :
			NewWidth(newWidth), NewHeight(newHeight) {}

		// The new height of the window
		uint32_t NewWidth;

		// The new width of the window
		uint32_t NewHeight;
	};

	struct NovaAPI WindowCreateParams
	{
		WindowCreateParams(uint32_t width, uint32_t height, const string& title) :
			InitialWidth(width), InitialHeight(height), Title(title) {}

		uint32_t InitialWidth;
		uint32_t InitialHeight;
		string Title;
		bool VSync = true;
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
		/// Gets the current width of this window
		/// </summary>
		/// <returns>The current width of this window</returns> 
		virtual uint32_t GetWidth() const = 0;

		/// <summary>
		/// Gets the current height of this window
		/// </summary>
		/// <returns>The current height of this window</returns> 
		virtual uint32_t GetHeight() const = 0;

		/// <summary>
		/// Gets the current title of this window
		/// </summary>
		/// <returns>The current title of this window</returns> 
		virtual const string& GetTitle() const = 0;

		/// <summary>
		/// Enables/disables vsync for this window
		/// </summary>
		/// <param name="enabled">If true, vsync will be enabled</param>
		virtual void SetVSyncEnabled(bool enabled) = 0;

		/// <summary>
		/// Gets if vsync is enabled for this window
		/// </summary>
		/// <returns>True if vsync is enabled for this window</returns>
		virtual bool GetVSyncEnabled() const = 0;

		/// <summary>
		/// Tells this window to close
		/// </summary>
		virtual void Close() = 0;

		/// <summary>
		/// Gets this window's graphics context
		/// </summary>
		/// <returns>This window's graphics context</returns>
		virtual Ref<Rendering::GraphicsContext> GetGraphicsContext() = 0;

		virtual void* GetNativeWindow() = 0;

	public:
		/// Invoked when this window is trying to close
		EventSource<WindowClosingEvent> OnClosing;

		/// Invoked when this window is closed
		EventSource<Event> OnClosed;

		/// Invoked when this window is resized
		EventSource<WindowResizedEvent> OnResized;
	};
}
#pragma once

#include "Nova/Core/EngineAPI.h"
#include "../Window.h"

#include <SDL.h>

namespace Nova::Windowing
{
	/// <summary>
	/// A GUI window implemented with SDL
	/// </summary>
	class NovaAPI SDLWindow : public Window
	{
	public:
		SDLWindow(const WindowCreateParams& params);
		~SDLWindow();

		// Window ----------
	public:
		virtual void SetSize(const Vector2i& size) override;
		virtual Vector2i GetSize() const override;

		virtual void SetPosition(const Vector2i& position) override;
		virtual Vector2i GetPosition() const override;

		virtual void SetTitle(const string& title) override;
		virtual string GetTitle() const override;

		virtual void SetVSync(bool enabled) override;
		virtual bool GetVSync() const override;

		virtual void Show() override;
		virtual void Hide() override;
		virtual bool GetIsVisible() const override;

		virtual void Focus() override;
		virtual bool GetIsFocused() const override;

		virtual bool GetIsMinimized() const override;

		virtual void SetOpacity(double opacity) override;
		virtual double GetOpacity() const override;

		virtual void* GetBackendWindowHandle() const override { return m_Window; }
		virtual void* GetPlatformWindowHandle() const override;

		// Window ----------

	public:
		/// <summary>
		/// Gets an SDLWindow from an internal SDL_Window
		/// </summary>
		/// <params name="window">The internal window</params>
		/// <returns>The SDLWindow for the given internal window</returns>
		static SDLWindow* GetWindowForInternalWindow(SDL_Window* window);

	private:
		/// <summary>
		/// The name of the pointer data for the SDLWindow pointer
		/// </summary>
		static const char* s_WindowPointerText;

	public:
		/// <summary>
		/// Allows this window to handle the given event
		/// </summary>
		/// <param name="e">The event to handle</param>
		void HandleEvent(SDL_Event* e);

	private:
		/// <summary>
		/// Creates the internal SDL_Window
		/// </summary>
		/// <param name="params">Parameters for the window</param>
		void CreateInternalWindow(const WindowCreateParams& params);

		/// <summary>
		/// Sets up a swapchain for this window
		/// </summary>
		void SetupWindowSwapchain();

		/// <summary>
		/// Cleans up the internal SDL_Window
		/// </summary>
		void CleanupInternalWindow();

	private:
		/// <summary>
		/// The internal window
		/// </summary>
		SDL_Window* m_Window;

		/// <summary>
		/// The internal window surface that can be drawn to
		/// </summary>
		SDL_Surface* m_WindowSurface;
	};
}
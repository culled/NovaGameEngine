#include "SDLWindow.h"

#include "../WindowExceptions.h"
#include "Nova/Core/Engine/Engine.h"

#include <SDL.h>

namespace Nova::Windowing
{
	SDLWindow::SDLWindow(const WindowCreateParams& params)
	{
		CreateInternalWindow(params);
	}

	SDLWindow::~SDLWindow()
	{
		CleanupInternalWindow();
	}

	// Window ----------
	void SDLWindow::SetSize(const Vector2i& size)
	{
		SDL_SetWindowSize(m_Window, size.X, size.Y);
	}

	Vector2i SDLWindow::GetSize() const
	{
		Vector2i size;

		SDL_GetWindowSize(m_Window, &size.X, &size.Y);

		return size;
	}

	void SDLWindow::SetPosition(const Vector2i& position)
	{
		SDL_SetWindowPosition(m_Window, position.X, position.Y);
	}

	Vector2i SDLWindow::GetPosition() const
	{
		Vector2i pos;

		SDL_GetWindowPosition(m_Window, &pos.X, &pos.Y);

		return pos;
	}

	void SDLWindow::SetTitle(const string& title)
	{
		SDL_SetWindowTitle(m_Window, title.c_str());
	}

	string SDLWindow::GetTitle() const
	{
		return string(SDL_GetWindowTitle(m_Window));
	}

	void SDLWindow::SetVSync(bool enabled)
	{
		// TODO
		throw Exception("Not implemented");
	}

	bool SDLWindow::GetVSync() const
	{
		// TODO
		throw Exception("Not implemented");
		return false;
	}

	void SDLWindow::Show()
	{
		SDL_ShowWindow(m_Window);
	}

	void SDLWindow::Hide()
	{
		SDL_HideWindow(m_Window);
	}

	bool SDLWindow::GetIsVisible() const
	{
		return SDL_GetWindowFlags(m_Window) & SDL_WINDOWEVENT_SHOWN;
	}

	void SDLWindow::Focus()
	{
		SDL_RaiseWindow(m_Window);
	}

	bool SDLWindow::GetIsFocused() const
	{
		return SDL_GetWindowFlags(m_Window) & SDL_WINDOW_INPUT_FOCUS;
	}

	bool SDLWindow::GetIsMinimized() const
	{
		return SDL_GetWindowFlags(m_Window) & SDL_WINDOW_MINIMIZED;
	}

	void SDLWindow::SetOpacity(double opacity)
	{
		SDL_SetWindowOpacity(m_Window, (float)opacity);
	}

	double SDLWindow::GetOpacity() const
	{
		float opacity;

		SDL_GetWindowOpacity(m_Window, &opacity);

		return (double)opacity;
	}

	void* SDLWindow::GetPlatformWindowHandle() const
	{
		// TODO
		throw Exception("Not implemented");
		return nullptr;
	}

	// Window ----------

	SDLWindow* SDLWindow::GetWindowForInternalWindow(SDL_Window* window)
	{
		return (SDLWindow*)SDL_GetWindowData(window, s_WindowPointerText);
	}

	const char* SDLWindow::s_WindowPointerText = "SDLWindow";

	void SDLWindow::HandleEvent(SDL_Event* e)
	{
		if (e->window.event == SDL_WINDOWEVENT_CLOSE)
		{
			Close();
		}
	}

	void SDLWindow::CreateInternalWindow(const WindowCreateParams& params)
	{
		int x = SDL_WINDOWPOS_UNDEFINED;
		int y = SDL_WINDOWPOS_UNDEFINED;

		if (params.InitialPosition != Vector2i::MAX)
		{
			x = params.InitialPosition.X;
			y = params.InitialPosition.Y;
		}

		m_Window = SDL_CreateWindow(
			params.Title.c_str(),
			x,
			y,
			params.InitialSize.X,
			params.InitialSize.Y,
			SDL_WINDOW_HIDDEN
		);

		if (m_Window == NULL)
		{
			throw WindowCreateException(FormatString("Unable to create SDLWindow: {0}", SDL_GetError()));
		}

		SDL_SetWindowData(m_Window, s_WindowPointerText, this);

		SetupWindowSwapchain();

		Engine::Get()->Log(LogLevel::Verbose, "SDLWindow successfully created");
	}

	void SDLWindow::SetupWindowSwapchain()
	{
		m_WindowSurface = SDL_GetWindowSurface(m_Window);
	}

	void SDLWindow::CleanupInternalWindow()
	{
		if (m_Window)
			SDL_DestroyWindow(m_Window);

		Engine::Get()->Log(LogLevel::Verbose, "SDLWindow destroyed");
	}
}
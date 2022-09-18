#include "SDLWindowingBackend.h"

#include "SDLWindow.h"
#include "../WindowingBackendExceptions.h"
#include "Nova/Core/Engine/Engine.h"

#include <SDL.h>

namespace Nova::Windowing
{
	SDLWindowingBackend::SDLWindowingBackend()
	{
		if(!s_SDLInitialized)
			InitSDL();

		Engine::Get()->Log(LogLevel::Verbose, "SDLWindowingBackend initialized");
	}

	SDLWindowingBackend::~SDLWindowingBackend()
	{
		if(s_SDLInitialized)
			ShutdownSDL();

		Engine::Get()->Log(LogLevel::Verbose, "SDLWindowingBackend destroyed");
	}

	// WindowingBackend ----------
	Ref<Window> SDLWindowingBackend::CreateWindow(const WindowCreateParams& params)
	{
		Engine::Get()->Log(LogLevel::Verbose, "Creating SDLWindow");
		return MakeRef<SDLWindow>(params);
	}

	void SDLWindowingBackend::Tick(double deltaTime)
	{
		SDL_Event e;

		// Handle all queued events
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_WINDOWEVENT)
			{
				DispatchWindowEvent(&e);
			}
		}
	}

	// WindowingBackend ----------

	void SDLWindowingBackend::InitSDL()
	{
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			throw WindowingBackendInitException(FormatString("Failed to initialize SDL: {0}", SDL_GetError()));

			SDL_Quit();
			return;
		}

		s_SDLInitialized = true;

		Engine::Get()->Log(LogLevel::Verbose, "SDL initialized successfully");
	}

	void SDLWindowingBackend::ShutdownSDL()
	{
		SDL_Quit();

		s_SDLInitialized = false;

		Engine::Get()->Log(LogLevel::Verbose, "Shutdown SDL");
	}

	bool SDLWindowingBackend::s_SDLInitialized = false;

	void SDLWindowingBackend::DispatchWindowEvent(SDL_Event* e) const
	{
		// Find the window that the event is for
		auto sdlWindow = SDL_GetWindowFromID(e->window.windowID);

		if (sdlWindow == NULL)
		{
			return;
		}

		// Dispatch the event to the proper window
		auto window = SDLWindow::GetWindowForInternalWindow(sdlWindow);

		if (!window)
		{
			Engine::Get()->Log(LogLevel::Error, "The SDLWindow for window {0} is null", e->window.windowID);
			return;
		}

		window->HandleEvent(e);
	}
}
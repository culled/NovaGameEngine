#pragma once

#include "Nova/Core/EngineAPI.h"
#include "Nova/Core/Types/Map.h"
#include "../WindowingBackend.h"

#include <SDL.h>

namespace Nova::Windowing
{
	/// <summary>
	/// A WindowingBackend that uses SDL 
	/// </summary>
	class NovaAPI SDLWindowingBackend : public WindowingBackend
	{
	public:
		SDLWindowingBackend();
		~SDLWindowingBackend();

		// WindowingBackend ----------
	public:
		virtual Ref<Window> CreateWindow(const WindowCreateParams& params) override;
		virtual void Tick(double deltaTime) override;

		// WindowingBackend ----------

	private:
		/// <summary>
		/// Initializes SDL
		/// </summary>
		static void InitSDL();

		/// <summary>
		/// Shutsdown SDL
		/// </summary>
		static void ShutdownSDL();

	private:
		/// <summary>
		/// True if SDL has been initialized
		/// </summary>
		static bool s_SDLInitialized;

	private:
		/// <summary>
		/// Dispatches an event to the correct window
		/// </summary>
		/// <param name="e">The event to dispatch</param>
		void DispatchWindowEvent(SDL_Event* e) const;
	};
}
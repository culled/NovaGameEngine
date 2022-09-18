#pragma once

#include "Nova/Core/EngineAPI.h"
#include "Nova/Core/Services/EngineService.h"
#include "Nova/Core/Events/TickListener.h"
#include "Nova/Core/Types/List.h"
#include "WindowingBackend.h"
#include "Window.h"

namespace Nova::Windowing
{
	class NovaAPI WindowingService : public EngineService
	{
	public:
		WindowingService(WindowingAPI windowingAPI);
		~WindowingService();

	public:
		/// <summary>
		/// Creates a window for use with this service
		/// </summary>
		/// <param name="createParams">The parameters for the new window</param>
		/// <returns>The created window</returns>
		Ref<Window> CreateWindow(const WindowCreateParams& createParams);

		/// <summary>
		/// Sets the given window as the main window
		/// </summary>
		/// <param name="window">The new main window</param>
		void SetMainWindow(const Ref<Window>& window);

	private:
		/// <summary>
		/// Creates a WindowingBackend for this service
		/// </summary>
		/// <param name="api">The api to use</param>
		void CreateBackend(WindowingAPI api);

		/// <summary>
		/// Creates TickListeners for this service
		/// </summary>
		void CreateTickListeners();

		/// <summary>
		/// Ticks this service
		/// </summary>
		/// <param name="deltaTime">The time between ticks</param>
		void Tick(double deltaTime);

		/// <summary>
		/// Queues a window to be destroyed
		/// </summary>
		/// <param name="window">The window that should be released</param>
		void QueueWindowRelease(const Ref<Window>& window);

	private:
		/// <summary>
		/// A list of active windows
		/// </summary>
		List<Ref<Window>> m_Windows;

		/// <summary>
		/// The main window of the application
		/// </summary>
		Ref<Window> m_MainWindow;

		/// <summary>
		/// A list of windows pending to be destroyed
		/// </summary>
		List<Ref<Window>> m_WindowsToRelease;

		/// <summary>
		/// The windowing backend
		/// </summary>
		ManagedPtr<WindowingBackend> m_Backend;

		/// <summary>
		/// Our main tick listener
		/// </summary>
		Ref<TickListener> m_TickListener;

		friend class Window;
	};
}
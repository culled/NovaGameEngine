#include "WindowingService.h"

#include "Nova/Core/Services/EngineServiceExceptions.h"
#include "Nova/Core/Engine/Engine.h"

#include "SDL/SDLWindowingBackend.h"

namespace Nova::Windowing
{
	WindowingService::WindowingService(WindowingAPI windowingAPI)
	{
		CreateBackend(windowingAPI);
		CreateTickListeners();

		Engine::Get()->Log(LogLevel::Verbose, "WindowingService initialized");
	}

	WindowingService::~WindowingService()
	{
		// Release all windows
		m_Windows.clear();
		m_WindowsToRelease.clear();
		m_MainWindow.reset();

		// Release the backend
		m_Backend.reset();

		// Clean up our tick listener
		RemoveListenerFromMainLoop(m_TickListener);

		Engine::Get()->Log(LogLevel::Verbose, "WindowingService destroyed");
	}

	Ref<Window> WindowingService::CreateWindow(const WindowCreateParams& createParams)
	{
		auto window = m_Backend->CreateWindow(createParams);

		m_Windows.push_back(window);

		// Make the first window the main window
		if (!m_MainWindow)
			SetMainWindow(window);

		return window;
	}

	void WindowingService::SetMainWindow(const Ref<Window>& window)
	{
		if (m_MainWindow)
			m_MainWindow->SetMainWindow(false);

		m_MainWindow = window;
		m_MainWindow->SetMainWindow(true);
	}

	void WindowingService::CreateBackend(WindowingAPI api)
	{
		switch (api)
		{
		case WindowingAPI::SDL:
			Engine::Get()->Log(LogLevel::Verbose, "Creating SDLWindowingBackend");
			m_Backend = MakeManagedPtr<SDLWindowingBackend>();
			break;

		default:
			throw EngineServiceInitException("Unsupported API");
		}
	}

	void WindowingService::CreateTickListeners()
	{
		m_TickListener = CreateTickListener(-1000, &WindowingService::Tick);
		AddListenerToMainLoop(m_TickListener);
	}

	void WindowingService::Tick(double deltaTime)
	{
		// Release all pending windows
		m_WindowsToRelease.clear();

		m_Backend->Tick(deltaTime);
	}

	void WindowingService::QueueWindowRelease(const Ref<Window>& window)
	{
		auto it = std::find_if(m_Windows.begin(), m_Windows.end(), [window](const Ref<Window>& otherWindow)
			{
				return window.get() == otherWindow.get();
			});

		if (it != m_Windows.end())
		{
			// Shutdown the engine if our main window was closed
			if (window == m_MainWindow)
			{
				m_MainWindow.reset();
				Engine::Get()->Stop();
			}

			// We defer the release of the window until our next tick
			m_WindowsToRelease.push_back(*it);
			m_Windows.erase(it);	
		}
	}
}
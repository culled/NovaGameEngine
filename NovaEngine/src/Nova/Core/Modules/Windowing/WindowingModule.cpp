#include "WindowingModule.h"

#include "Nova/Core/App/App.h"

#include "Backends/GLFW/GLFWWindowingBackend.h"

namespace Nova::Windowing
{
	WindowCreatedEvent::WindowCreatedEvent(const Ref<Windowing::Window>& window) :
		Window(window)
	{}

	WindowDestroyedEvent::WindowDestroyedEvent(const Ref<Windowing::Window>& window) :
		Window(window)
	{}

	WindowingModule::WindowingModule(int tickOffset, WindowingAPI backendAPI) :
		AppModule(tickOffset)
	{
		if (s_Instance.lock())
		{
			// Can't have more than one windowing module!
			throw AppModuleInitException("A WindowModule is already in use");
		}

		App::LogCore(LogLevel::Verbose, "********** Initializing WindowingModule... **********");

		SetListenForTicks(true);
		CreateBackend(backendAPI);

		App::LogCore(LogLevel::Verbose, "********** WindowingModule initialized successfully **********");
	}

	WindowingModule::~WindowingModule()
	{
		// Release all windows
		m_Windows.clear();
		m_MainWindow = nullptr;

		App::LogCore(LogLevel::Verbose, "********** WindowingModule destroyed **********");
	}

	// RefCounted ----------
	void WindowingModule::Init()
	{
		s_Instance = GetSelfWeakRef<WindowingModule>();
	}

	// RefCounted ----------

	// TickListener ----------
	void WindowingModule::Tick(double deltaTime)
	{
		// Destroy pending windows
		m_DeferredWindowsToDestroy.clear();

		m_WindowingBackend->Tick(deltaTime);
	}

	// TickListener ----------

	WeakRef<WindowingModule> WindowingModule::s_Instance;

	WeakRef<Window> WindowingModule::CreateWindow(const WindowCreateParams& createParams)
	{
		auto window = m_WindowingBackend->CreateWindow(createParams);
		m_Windows.push_back(window);

		// Make this window the main window if none exists yet
		if (!m_MainWindow)
			SetMainWindow(window);

		OnWindowCreated.EmitAnonymous(window);

		return window;
	}

	void WindowingModule::DestroyWindow(Ref<Window> window)
	{
		auto it = std::find_if(m_Windows.begin(), m_Windows.end(), [window](const Ref<Window>& other) {
			return window.get() == other.get();
			});

		if (it == m_Windows.end())
			return;

		// Keep the reference to the window alive until the next tick
		m_DeferredWindowsToDestroy.push_back(window);

		m_Windows.erase(it);
		OnWindowDestroyed.EmitAnonymous(window);
	}

	void WindowingModule::SetMainWindow(Ref<Window> window)
	{
		if (m_MainWindow)
		{
			m_MainWindow->SetMainWindow(false);
		}

		m_MainWindow = window;

		if (m_MainWindow)
		{
			m_MainWindow->SetMainWindow(true);
		}
	}

	void WindowingModule::CreateBackend(WindowingAPI backendAPI)
	{
		switch (backendAPI)
		{
		case WindowingAPI::GLFW:
			m_WindowingBackend = MakeRef<GLFWWindowingBackend>();
			break;
		default:
			throw Exception("Unsupported windowing API");
		}
	}
}
#include "WindowingModule.h"

#include "Nova/Core/App/App.h"

namespace Nova::Windowing
{
	WindowingModule::WindowingModule(int tickOffset) :
		AppModule(tickOffset)
	{
		if (s_Instance.lock())
		{
			// Can't have more than one display module!
			throw AppModuleInitException("A WindowModule is already in use");
		}
	}

	WindowingModule::~WindowingModule()
	{
		ReleaseAllWindows();

		App::LogCore(LogLevel::Verbose, "DisplayModule destroyed");
	}

	// RefCounted ----------
	void WindowingModule::Init()
	{
		s_Instance = GetSelfWeakRef<WindowingModule>();
		App::LogCore(LogLevel::Verbose, "DisplayModule created successfully");
	}

	// RefCounted ----------

	Ref<WindowingModule> WindowingModule::Get()
	{
		return s_Instance.lock();
	}

	WeakRef<WindowingModule> WindowingModule::s_Instance;

	Ref<Window> WindowingModule::CreateAndAddWindow(const WindowCreateParams& createParams)
	{
		Ref<Window> window = CreateWindow(createParams);

		// If we have no current windows, make this one the main window
		if (m_Windows.size() == 0)
		{
			SetMainWindow(window);
		}

		m_Windows.push_back(window);

		return window;
	}

	void WindowingModule::SetMainWindow(Ref<Window> window)
	{
		Ref<WindowingModule> self = GetSelfRef<WindowingModule>();

		if (m_MainWindow)
		{
			m_MainWindow->OnClosing.Disconnect(self, &WindowingModule::MainWindowClosingCallback);
		}

		m_MainWindow = window;

		m_MainWindow->OnClosing.Connect(self, &WindowingModule::MainWindowClosingCallback);
	}

	void WindowingModule::WindowClosed(Ref<Window> window)
	{
		auto it = std::find_if(m_Windows.begin(), m_Windows.end(), [window](const Ref<Window>& other) {
			return window.get() == other.get();
		});

		if(it != m_Windows.end())
		{
			m_Windows.erase(it);
		}
	}

	void WindowingModule::ReleaseAllWindows()
	{
		m_Windows.clear();
		m_MainWindow = nullptr;
	}

	void WindowingModule::MainWindowClosingCallback(WindowClosingEvent& e)
	{
		// Only close the main window once the application quits
		e.ShouldClose = false;

		App::Get()->Quit();
	}
}
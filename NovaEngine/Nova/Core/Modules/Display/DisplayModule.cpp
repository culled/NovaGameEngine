#include "DisplayModule.h"

#include "Nova/Core/App/App.h"

namespace Nova::Display
{
	WeakRef<DisplayModule> DisplayModule::s_Instance;

	DisplayModule::DisplayModule(int tickOffset) : AppModule(tickOffset)
	{
		if (s_Instance.lock())
		{
			// Can't have more than one display module!
			throw Exception();
		}
	}

	DisplayModule::~DisplayModule()
	{
		// Release all windows
		m_Windows.clear();
		m_MainWindow = nullptr;

		App::LogCore("DisplayModule destroyed", LogLevel::Verbose);
	}

	void DisplayModule::Init()
	{
		s_Instance = GetWeakRef<DisplayModule>();
		App::LogCore("DisplayModule created successfully", LogLevel::Verbose);
	}

	Ref<DisplayModule> DisplayModule::Get()
	{
		return s_Instance.lock();
	}

	Ref<Window> DisplayModule::CreateAndAddWindow(uint32_t width, uint32_t height, const string& title)
	{
		Ref<Window> window = CreateWindow(width, height, title);

		if (m_Windows.size() == 0)
		{
			SetMainWindow(window);
		}

		m_Windows.push_back(window);

		return window;
	}

	void DisplayModule::SetMainWindow(Ref<Window> window)
	{
		auto self = GetRef<DisplayModule>();

		if (m_MainWindow)
		{
			m_MainWindow->OnClosed.Disconnect(self, &DisplayModule::MainWindowClosedCallback);
		}

		m_MainWindow = window;

		m_MainWindow->OnClosed.Connect(self, &DisplayModule::MainWindowClosedCallback);
	}

	void DisplayModule::MainWindowClosedCallback(Ref<Event> e)
	{
		App::Get()->Quit();
	}
}
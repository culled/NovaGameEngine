#include "WindowingModule.h"

#include "Nova/Core/App/App.h"

#include "Backends/GLFW/GLFWWindowingBackend.h"

namespace Nova::Windowing
{
	WindowingModule::WindowingModule(int tickOffset, WindowingAPI backendAPI) :
		AppModule(tickOffset)
	{
		App::LogCore(LogLevel::Verbose, "********** Initializing WindowingModule... **********");

		SetListenForTicks(true);

		if (s_Instance.lock())
		{
			// Can't have more than one windowing module!
			throw AppModuleInitException("A WindowModule is already in use");
		}

		CreateBackend(backendAPI);

		App::LogCore(LogLevel::Verbose, "********** WindowingModule initialized successfully **********");
	}

	WindowingModule::~WindowingModule()
	{
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
		m_WindowingBackend->Tick(deltaTime);
	}

	// TickListener ----------

	WeakRef<WindowingModule> WindowingModule::s_Instance;

	Ref<Window> WindowingModule::CreateAndAddWindow(const WindowCreateParams& createParams)
	{
		return m_WindowingBackend->CreateAndAddWindow(createParams);
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
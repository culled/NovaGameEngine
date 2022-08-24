#include "GLFWWindowingBackend.h"

#include "Nova/Core/App/App.h"
#include "Nova/Core/Modules/AppModuleException.h"
#include "GLFWWindow.h"

#include "GLFW/glfw3.h"

namespace Nova::Windowing
{
	GLFWWindowingBackend::GLFWWindowingBackend()
	{
		App::LogCore(LogLevel::Verbose, "Created GLFWWindowingBackend");

		if (!s_GLFWInitialized)
		{
			InitGLFW();
		}
	}

	GLFWWindowingBackend::~GLFWWindowingBackend()
	{
		// Release all windows
		m_Windows.clear();
		m_MainWindow.reset();

		if (s_GLFWInitialized)
		{
			TerminateGLFW();
		}

		App::LogCore(LogLevel::Verbose, "Destroyed GLFWWindowingBackend");
	}

	// WindowingBackend ----------

	Ref<Window> GLFWWindowingBackend::CreateAndAddWindow(const WindowCreateParams& createParams)
	{
		auto window = MakeRef<GLFWWindow>(createParams);

		// If we have no current windows, make this one the main window
		if (m_Windows.size() == 0)
		{
			SetMainWindow(window);
		}

		m_Windows.push_back(window);

		return window;
	}

	void GLFWWindowingBackend::RemoveWindow(Ref<Window> window)
	{
		auto it = std::find_if(m_Windows.begin(), m_Windows.end(), [window](const Ref<Window>& other) {
			return window.get() == other.get();
			});

		if (it != m_Windows.end())
		{
			m_Windows.erase(it);
		}
	}

	void GLFWWindowingBackend::SetMainWindow(Ref<Window> window)
	{
		Ref<GLFWWindowingBackend> self = GetSelfRef<GLFWWindowingBackend>();

		if (m_MainWindow)
		{
			m_MainWindow->OnClosing.Disconnect(self, &GLFWWindowingBackend::MainWindowClosingCallback);
		}

		m_MainWindow = window;

		// Set the tree context of the main window's graphics context to the app's NodeTree
		m_MainWindow->GetGraphicsContext()->SetNodeTreeContext(App::Get()->GetNodeTree());

		m_MainWindow->OnClosing.Connect(self, &GLFWWindowingBackend::MainWindowClosingCallback);
	}

	void GLFWWindowingBackend::Tick(double deltaTime)
	{
		if (!s_GLFWInitialized)
			return;

		glfwPollEvents();
	}

	// WindowingBackend ----------

	void GLFWWindowingBackend::InitGLFW()
	{
		if (s_GLFWInitialized)
			return;

		if (!glfwInit())
		{
			throw AppModuleInitException("Unable to initialize GLFW");
		}

		s_GLFWInitialized = true;
		App::LogCore(LogLevel::Verbose, "GLFW initialized");
	}

	void GLFWWindowingBackend::TerminateGLFW()
	{
		if (!s_GLFWInitialized)
			return;

		glfwTerminate();

		s_GLFWInitialized = false;
		App::LogCore(LogLevel::Verbose, "GLFW terminated");
	}

	bool GLFWWindowingBackend::s_GLFWInitialized = false;

	void GLFWWindowingBackend::MainWindowClosingCallback(WindowClosingEvent& e)
	{
		// Only close the main window once the application quits
		e.ShouldClose = App::Get()->Quit();
	}
}
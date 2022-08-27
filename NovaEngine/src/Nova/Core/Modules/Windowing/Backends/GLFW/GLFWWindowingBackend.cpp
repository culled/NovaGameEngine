#include "GLFWWindowingBackend.h"

#include "Nova/Core/App/App.h"
#include "Nova/Core/Modules/AppModuleException.h"
#include "GLFWWindow.h"
#include "GLFWMonitor.h"

#include <GLFW/glfw3.h>

namespace Nova::Windowing
{
	GLFWWindowingBackend::GLFWWindowingBackend()
	{
		App::LogCore(LogLevel::Verbose, "Created GLFWWindowingBackend");

		if (!s_GLFWInitialized)
		{
			InitGLFW();
		}

		InitializeMonitors();
	}

	GLFWWindowingBackend::~GLFWWindowingBackend()
	{
		if (s_GLFWInitialized)
		{
			TerminateGLFW();
		}

		App::LogCore(LogLevel::Verbose, "Destroyed GLFWWindowingBackend");
	}

	// WindowingBackend ----------

	void GLFWWindowingBackend::MonitorCallback(GLFWmonitor* monitor, int eventType)
	{
		GLFWWindowingBackend* backend = static_cast<GLFWWindowingBackend*>(glfwGetMonitorUserPointer(monitor));

		if (eventType == GLFW_CONNECTED)
		{
			backend->MonitorConnected(monitor);
		}
		else if (eventType == GLFW_DISCONNECTED)
		{
			backend->MonitorDisconnected(monitor);
		}
	}

	Ref<Window> GLFWWindowingBackend::CreateWindow(const WindowCreateParams& createParams)
	{
		if (!s_GLFWInitialized)
			throw Exception("GLFW has not been initialized yet!");

		return MakeRef<GLFWWindow>(createParams);
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

	void GLFWWindowingBackend::InitializeMonitors()
	{
		int count = 0;
		GLFWmonitor** monitors = glfwGetMonitors(&count);

		for (int i = 0; i < count; i++)
		{
			MonitorConnected(monitors[i]);
		}

		glfwSetMonitorCallback(&GLFWWindowingBackend::MonitorCallback);
	}

	void GLFWWindowingBackend::MonitorConnected(GLFWmonitor* monitor)
	{
		glfwSetMonitorUserPointer(monitor, this);
		m_Monitors.push_back(MakeRef<GLFWMonitor>(monitor));
	}

	void GLFWWindowingBackend::MonitorDisconnected(GLFWmonitor* monitor)
	{
		auto it = std::find_if(m_Monitors.begin(), m_Monitors.end(), [monitor](const Ref<Monitor>& other) {
			if (Ref<GLFWMonitor> otherMonitor = dynamic_pointer_cast<GLFWMonitor>(other))
			{
				return otherMonitor->GetInternalMonitor() == monitor;
			}

			return false;
		});

		if (it != m_Monitors.end())
		{
			m_Monitors.erase(it);
		}
	}
}
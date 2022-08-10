#include "GLFWWindow.h"

#include "Nova/Core/App/App.h"
#include "Nova/Core/Modules/AppModuleException.h"
#include "Nova/Core/Modules/Display/DisplayModule.h"

#include "Vendor/glfw/include/GLFW/glfw3.h"

namespace Nova::Display
{
	void GLFWWindow::CloseCallback(GLFWwindow* internalWindow)
	{
		GLFWWindow* window = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(internalWindow));

		Ref<WindowClosingEvent> closingEvent = MakeRef<WindowClosingEvent>();
		window->OnClosing.Emit(closingEvent);

		if (closingEvent->ShouldClose)
		{
			glfwSetWindowShouldClose(internalWindow, 1);
			window->OnClosed.Emit(MakeRef<Event>());
		}
		else
		{
			glfwSetWindowShouldClose(internalWindow, 0);
		}
	}

	GLFWWindow::GLFWWindow(uint32_t width, uint32_t height, const string& title) :
		m_Width(width), m_Height(height), m_Title(title)
	{
		CreateInternalWindow();
	}

	GLFWWindow::~GLFWWindow()
	{
		App::LogCore("Destroyed a GLFW window", LogLevel::Verbose);
	}

	void GLFWWindow::CreateInternalWindow()
	{
		// Tell GLFW to create a window
		GLFWwindow* internalWindow = glfwCreateWindow(int(m_Width), int(m_Height), m_Title.c_str(), NULL, NULL);

		if (!internalWindow)
		{
			App::LogCore("Failed to create GLFW window", LogLevel::Verbose);
			throw Exception("Failed to create GLFW window");
		}

		// Save a pointer to this window object in the GLFW window
		glfwSetWindowUserPointer(internalWindow, this);

		glfwSetWindowCloseCallback(internalWindow, &GLFWWindow::CloseCallback);

		m_InternalWindow.reset(internalWindow);

		App::LogCore("Created a GLFW window", LogLevel::Verbose);
	}

	void GLFWWindow::SetVSyncEnabled(bool enabled)
	{
		m_VSync = enabled;

		if (enabled)
		{
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}
	}
}
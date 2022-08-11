#include "GLFWWindow.h"

#include "Nova/Core/App/App.h"
#include "Nova/Core/Modules/AppModuleException.h"
#include "Nova/Core/Modules/Windowing/WindowingModule.h"

#include "Vendor/glfw/include/GLFW/glfw3.h"

namespace Nova::Windowing
{
	GLFWWindow::GLFWWindow(const WindowCreateParams& createParams) :
		m_Width(createParams.InitialWidth), m_Height(createParams.InitialHeight), m_Title(createParams.Title)
	{
		CreateInternalWindow();
		SetVSyncEnabled(createParams.VSync);
	}

	GLFWWindow::~GLFWWindow()
	{
		App::LogCore("Destroyed a GLFW window", LogLevel::Verbose);
	}

	// Window ----------
	void GLFWWindow::Close()
	{
		glfwSetWindowShouldClose(m_InternalWindow.get(), GL_TRUE);
	}

	// Window ----------

	void GLFWWindow::CloseCallback(GLFWwindow* internalWindow)
	{
		GLFWWindow* window = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(internalWindow));

		WindowClosingEvent closingEvent;
		window->OnClosing.Emit(closingEvent);

		if (closingEvent.ShouldClose)
		{
			glfwSetWindowShouldClose(internalWindow, GL_TRUE);

			window->OnClosed.EmitAnonymous();
			WindowingModule::Get()->WindowClosed(window->GetSelfRef<GLFWWindow>());
		}
		else
		{
			glfwSetWindowShouldClose(internalWindow, GL_FALSE);
		}
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

		// Set the callback for when the window tries to close
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
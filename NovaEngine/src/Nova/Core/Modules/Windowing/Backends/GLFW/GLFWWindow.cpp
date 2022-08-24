#include "GLFWWindow.h"

#include "Nova/Core/App/App.h"
#include "Nova/Core/Modules/AppModuleException.h"
#include "Nova/Core/Modules/Rendering/RenderModule.h"
#include "Nova/Core/Modules/Windowing/WindowingModule.h"
#include "Nova/Core/Modules/Windowing/WindowingBackend.h"
#include "GLFWGraphicsContext.h"

#include "GLFW/glfw3.h"

namespace Nova::Windowing
{
	GLFWWindow::GLFWWindow(const WindowCreateParams& createParams) :
		m_Width(createParams.InitialWidth), m_Height(createParams.InitialHeight), m_Title(createParams.Title)
	{
		App::LogCore(LogLevel::Verbose, "********** Creating a GLFW window **********");

		CreateInternalWindow();
		SetVSyncEnabled(createParams.VSync);

		App::LogCore(LogLevel::Verbose, "********** Created a GLFW window successfully **********");
	}

	GLFWWindow::~GLFWWindow()
	{
		App::LogCore(LogLevel::Verbose, "********** Destroyed a GLFW window **********");
	}

	// Window ----------
	void GLFWWindow::Close()
	{
		glfwSetWindowShouldClose(m_InternalWindow.get(), 1);
	}

	// Window ----------

	void GLFWWindow::CloseCallback(GLFWwindow* internalWindow)
	{
		GLFWWindow* window = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(internalWindow));

		// Emit the closing event
		WindowClosingEvent closingEvent;
		window->OnClosing.Emit(closingEvent);

		if (closingEvent.ShouldClose)
		{
			glfwSetWindowShouldClose(internalWindow, 1);

			window->OnClosed.EmitAnonymous();
			WindowingModule::Get()->GetBackend()->RemoveWindow(window->GetSelfRef<GLFWWindow>());
		}
		else
		{
			glfwSetWindowShouldClose(internalWindow, 0);
		}
	}

	void GLFWWindow::ResizeCallback(GLFWwindow* internalWindow, int newWidth, int newHeight)
	{
		GLFWWindow* window = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(internalWindow));

		window->m_GraphicsContext->SetWidth((uint32_t)newWidth);
		window->m_GraphicsContext->SetHeight((uint32_t)newHeight);
	}

	void GLFWWindow::CreateInternalWindow()
	{
		Rendering::RenderingBackendAPI backendAPI = Rendering::RenderModule::Get()->GetBackend()->GetAPI();

		//if (backendAPI == Rendering::RenderingBackendAPI::Vulkan)
		//{
		//	// Tell GLFW to not create an OpenGL context by default if we're using Vulkan
		//	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		//}

		// Tell GLFW to create a window
		GLFWwindow* internalWindow = glfwCreateWindow(int(m_Width), int(m_Height), m_Title.c_str(), NULL, NULL);

		if (!internalWindow)
		{
			throw Exception("Failed to create GLFW window");
		}

		// Save a pointer to this window object in the GLFW window
		glfwSetWindowUserPointer(internalWindow, this);

		// Set the callback for when the window tries to close
		glfwSetWindowCloseCallback(internalWindow, &GLFWWindow::CloseCallback);

		// Set the callback for when the window is resized
		glfwSetWindowSizeCallback(internalWindow, &GLFWWindow::ResizeCallback);

		m_InternalWindow.reset(internalWindow);

		// Create a graphics context for this window
		m_GraphicsContext = MakeRef<GLFWGraphicsContext>(m_Width, m_Height);
		m_GraphicsContext->SetInternalContext(internalWindow);

		// Initialize our GraphicsContext
		Rendering::RenderModule::Get()->GetBackend()->InitGraphicsContext(m_GraphicsContext);
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
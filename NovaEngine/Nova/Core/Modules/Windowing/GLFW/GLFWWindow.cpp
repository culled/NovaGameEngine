#include "GLFWWindow.h"

#include "Nova/Core/App/App.h"
#include "Nova/Core/Modules/AppModuleException.h"
#include "Nova/Core/Modules/Windowing/WindowingModule.h"
#include "Nova/Core/Modules/Rendering/RenderModule.h"
#include "Nova/Core/Modules/Rendering/Backends/OpenGL/OpenGLGraphicsContext.h"

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
		App::LogCore(LogLevel::Verbose, "Destroyed a GLFW window");
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

		m_InternalWindow.reset(internalWindow);

		// Create a graphics context for this window
		m_GraphicsContext = Rendering::RenderModule::Get()->GetBackend()->CreateGraphicsContext();

		// Since GLFW contains an OpenGL context, and the rendering system is decoupled from the windowing system,
		// we need to do some workarounds to load our OpenGL extensions properly
		if (backendAPI == Rendering::RenderingBackendAPI::OpenGL)
		{
			// Set a callback in the graphics context to tell GLFW to make the corresponding window's context current
			m_GraphicsContext->SetMakeCurrentCallback([&]() {
				glfwMakeContextCurrent(m_InternalWindow.get());
			});

			// Make this window's context current
			m_GraphicsContext->MakeCurrent();

			// Load extensions into this context
			m_GraphicsContext->LoadExtensions(glfwGetProcAddress);
		}

		App::LogCore(LogLevel::Verbose, "Created a GLFW window");
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
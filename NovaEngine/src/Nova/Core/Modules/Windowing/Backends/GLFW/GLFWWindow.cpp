#include "GLFWWindow.h"

#include "Nova/Core/App/App.h"
#include "Nova/Core/Modules/AppModuleException.h"
#include "Nova/Core/Modules/Rendering/RenderModule.h"
#include "Nova/Core/Modules/Windowing/WindowingModule.h"
#include "Nova/Core/Modules/Windowing/WindowingBackend.h"
#include "GLFWGraphicsContext.h"

#include <GLFW/glfw3.h>

#ifdef _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h> // for glfwGetWin32Window()
#endif

namespace Nova::Windowing
{
	GLFWWindow::GLFWWindow(const WindowCreateParams& createParams)
	{
		App::LogCore(LogLevel::Verbose, "********** Creating a GLFW window **********");
		
		CreateInternalWindow(createParams);
		SetVSync(createParams.VSync);

		// Move the window to the initial position (if one was given)
		if (createParams.InitialPosition != Vector2i::ZERO)
		{
			SetPosition(createParams.InitialPosition);
		}

		App::LogCore(LogLevel::Verbose, "********** Created a GLFW window successfully **********");
	}

	GLFWWindow::~GLFWWindow()
	{
		glfwDestroyWindow(m_InternalWindowPtr);
		App::LogCore(LogLevel::Verbose, "********** Destroyed a GLFW window **********");
	}

	// Window ----------
	void GLFWWindow::SetSize(const Vector2i& size)
	{
		glfwSetWindowSize(m_InternalWindowPtr, size.X, size.Y);
	}

	Vector2i GLFWWindow::GetSize() const
	{
		Vector2i size;
		glfwGetWindowSize(m_InternalWindowPtr, &size.X, &size.Y);
		return size;
	}

	void GLFWWindow::SetPosition(const Vector2i& position)
	{
		glfwSetWindowPos(m_InternalWindowPtr, position.X, position.Y);
	}

	Vector2i GLFWWindow::GetPosition() const
	{
		Vector2i pos;
		glfwGetWindowPos(m_InternalWindowPtr, &pos.X, &pos.Y);
		return pos;
	}

	void GLFWWindow::SetTitle(const string& title)
	{
		glfwSetWindowTitle(m_InternalWindowPtr, title.c_str());
	}

	string GLFWWindow::GetTitle() const
	{
		string title;
		glfwSetWindowTitle(m_InternalWindowPtr, title.data());
		return title;
	}

	void GLFWWindow::SetVSync(bool enabled)
	{
		m_VSync = enabled;

		m_GraphicsContext->MakeCurrent();

		if (enabled)
		{
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}

		m_GraphicsContext->ReleaseCurrent();
	}

	void GLFWWindow::Show()
	{
		glfwShowWindow(m_InternalWindowPtr);
	}

	void GLFWWindow::Hide()
	{
		glfwHideWindow(m_InternalWindowPtr);
	}

	bool GLFWWindow::GetIsHidden() const
	{
		return glfwGetWindowAttrib(m_InternalWindowPtr, GLFW_VISIBLE) == GLFW_FALSE;
	}

	void GLFWWindow::Focus()
	{
		glfwFocusWindow(m_InternalWindowPtr);
	}

	bool GLFWWindow::GetIsFocused() const
	{
		return glfwGetWindowAttrib(m_InternalWindowPtr, GLFW_FOCUSED) == GLFW_TRUE;
	}

	bool GLFWWindow::GetIsMinimized() const
	{
		return glfwGetWindowAttrib(m_InternalWindowPtr, GLFW_ICONIFIED) == GLFW_TRUE;
	}

	void GLFWWindow::SetOpacity(double opacity)
	{
		glfwSetWindowOpacity(m_InternalWindowPtr, (float)opacity);
	}

	double GLFWWindow::GetOpacity() const
	{
		return (double)glfwGetWindowOpacity(m_InternalWindowPtr);
	}

	void* GLFWWindow::GetPlatformWindowHandle() const
	{
#ifdef _WIN32
		return glfwGetWin32Window(m_InternalWindowPtr);
#elif
		return nullptr;
#endif
	}

	void GLFWWindow::Close(bool forceClosed)
	{
		// Emit the closing event
		WindowClosingEvent closingEvent;
		OnClosing.Emit(closingEvent);

		if (closingEvent.ShouldClose || forceClosed)
		{
			// The main window only closes when the application quits
			if (m_IsMainWindow && !App::Get()->Quit())
			{
				glfwSetWindowShouldClose(m_InternalWindowPtr, 0);
				return;
			}

			glfwSetWindowShouldClose(m_InternalWindowPtr, 1);

			OnClosed.EmitAnonymous();
			WindowingModule::Get()->DestroyWindow(GetSelfRef<GLFWWindow>());
		}
		else
		{
			glfwSetWindowShouldClose(m_InternalWindowPtr, 0);
		}
	}

	// Window ----------

	void GLFWWindow::CloseCallback(GLFWwindow* internalWindow)
	{
		GLFWWindow* window = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(internalWindow));

		window->Close();
	}

	void GLFWWindow::ResizeCallback(GLFWwindow* internalWindow, int newWidth, int newHeight)
	{
		GLFWWindow* window = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(internalWindow));

		WindowResizedEvent e(Vector2i(newWidth, newHeight));
		window->OnResized.Emit(e);
	}

	void GLFWWindow::CreateInternalWindow(const WindowCreateParams& createParams)
	{
		Rendering::RenderingBackendAPI backendAPI = Rendering::RenderModule::Get()->GetBackend()->GetAPI();

		//if (backendAPI == Rendering::RenderingBackendAPI::Vulkan)
		//{
		//	// Tell GLFW to not create an OpenGL context by default if we're using Vulkan
		//	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		//}

		auto gfxContextShare = createParams.ShareGfxContext ? dynamic_pointer_cast<GLFWGraphicsContext>(createParams.ShareGfxContext) : nullptr;

		// Tell GLFW to create a window
		m_InternalWindowPtr = glfwCreateWindow(
			createParams.InitialSize.X, 
			createParams.InitialSize.Y,
			createParams.Title.c_str(),
			NULL, 
			gfxContextShare ? gfxContextShare->GetInternalContext() : NULL);

		if (!m_InternalWindowPtr)
		{
			throw Exception("Failed to create GLFW window");
		}

		// Save a pointer to this window object in the GLFW window
		glfwSetWindowUserPointer(m_InternalWindowPtr, this);

		// Set the callback for when the window tries to close
		glfwSetWindowCloseCallback(m_InternalWindowPtr, &GLFWWindow::CloseCallback);

		// Set the callback for when the window is resized
		glfwSetWindowSizeCallback(m_InternalWindowPtr, &GLFWWindow::ResizeCallback);

		// Create a graphics context for this window
		m_GraphicsContext = MakeRef<GLFWGraphicsContext>(m_InternalWindowPtr, gfxContextShare != nullptr);
	}
}
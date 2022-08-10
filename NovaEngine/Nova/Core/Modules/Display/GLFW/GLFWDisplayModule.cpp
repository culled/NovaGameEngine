#include "GLFWDisplayModule.h"

#include "Nova/Core/App/App.h"
#include "Nova/Core/Modules/AppModuleException.h"
#include "GLFWWindow.h"

#include "Vendor/glfw/include/GLFW/glfw3.h"

namespace Nova::Display
{
	bool GLFWDisplayModule::s_GLFWInitialized = false;

	void GLFWDisplayModule::InitGLFW()
	{
		if (s_GLFWInitialized)
			return;

		if (!glfwInit())
		{
			App::LogCore("Unable to initialize GLFW", LogLevel::Error);
			throw AppModuleInitException("Unable to initialize GLFW");
		}

		s_GLFWInitialized = true;
		App::LogCore("GLFW initialized", LogLevel::Verbose);
	}

	void GLFWDisplayModule::TerminateGLFW()
	{
		if (!s_GLFWInitialized)
			return;

		glfwTerminate();

		s_GLFWInitialized = false;
		App::LogCore("GLFW terminated", LogLevel::Verbose);
	}

	GLFWDisplayModule::GLFWDisplayModule(int tickOffset) : DisplayModule(tickOffset)
	{
		if (!s_GLFWInitialized)
		{
			InitGLFW();
		}
	}

	GLFWDisplayModule::~GLFWDisplayModule()
	{
		// HACK: Release all windows so that they release their glfw resources before we terminate it
		m_Windows.clear();
		m_MainWindow = nullptr;

		if (s_GLFWInitialized)
		{
			TerminateGLFW();
		}
	}

	Ref<Window> GLFWDisplayModule::CreateWindow(uint32_t width, uint32_t height, const string& title)
	{
		return MakeRef<GLFWWindow>(width, height, title);
	}

	void GLFWDisplayModule::Tick(double deltaTime)
	{
		if (!s_GLFWInitialized)
			return;

		glfwPollEvents();
	}
}
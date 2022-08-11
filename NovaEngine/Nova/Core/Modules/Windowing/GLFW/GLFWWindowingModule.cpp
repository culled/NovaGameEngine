#include "GLFWWindowingModule.h"

#include "Nova/Core/App/App.h"
#include "Nova/Core/Modules/AppModuleException.h"
#include "GLFWWindow.h"

#include "Vendor/glfw/include/GLFW/glfw3.h"

namespace Nova::Windowing
{
	GLFWWindowingModule::GLFWWindowingModule(int tickOffset) :
		WindowingModule(tickOffset)
	{
		SetListenForTicks(true);

		if (!s_GLFWInitialized)
		{
			InitGLFW();
		}
	}

	GLFWWindowingModule::~GLFWWindowingModule()
	{
		// HACK: Release all windows so that they release their glfw resources before we terminate it
		ReleaseAllWindows();

		if (s_GLFWInitialized)
		{
			TerminateGLFW();
		}
	}

	// TickListener ----------
	void GLFWWindowingModule::Tick(double deltaTime)
	{
		if (!s_GLFWInitialized)
			return;

		glfwPollEvents();
	}

	// TickListener ----------

	// WindowingModule ----------
	Ref<Window> GLFWWindowingModule::CreateWindow(const WindowCreateParams& createParams)
	{
		return MakeRef<GLFWWindow>(createParams);
	}

	// WindowingModule ----------

	void GLFWWindowingModule::InitGLFW()
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

	void GLFWWindowingModule::TerminateGLFW()
	{
		if (!s_GLFWInitialized)
			return;

		glfwTerminate();

		s_GLFWInitialized = false;
		App::LogCore("GLFW terminated", LogLevel::Verbose);
	}

	bool GLFWWindowingModule::s_GLFWInitialized = false;
}
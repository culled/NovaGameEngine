#include "GLFWWindowingModule.h"

#include "Nova/Core/App/App.h"
#include "Nova/Core/Modules/AppModuleException.h"
#include "GLFWWindow.h"

#include "GLFW/glfw3.h"

namespace Nova::Windowing
{
	GLFWWindowingModule::GLFWWindowingModule(int tickOffset) :
		WindowingModule(tickOffset)
	{
		SetListenForTicks(true);
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

	// RefCounted ----------
	void GLFWWindowingModule::Init()
	{
		App::LogCore(LogLevel::Verbose, "********** Initializing GLFWWindowingModule... **********");

		WindowingModule::Init();

		if (!s_GLFWInitialized)
		{
			InitGLFW();
		}

		App::LogCore(LogLevel::Verbose, "********** GLFWWindowingModule initialized successfully **********");
	}

	// RefCounted ----------

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
			App::LogCore(LogLevel::Error, "Unable to initialize GLFW");
			throw AppModuleInitException("Unable to initialize GLFW");
		}

		s_GLFWInitialized = true;
		App::LogCore(LogLevel::Verbose, "GLFW initialized");
	}

	void GLFWWindowingModule::TerminateGLFW()
	{
		if (!s_GLFWInitialized)
			return;

		glfwTerminate();

		s_GLFWInitialized = false;
		App::LogCore(LogLevel::Verbose, "GLFW terminated");
	}

	bool GLFWWindowingModule::s_GLFWInitialized = false;
}
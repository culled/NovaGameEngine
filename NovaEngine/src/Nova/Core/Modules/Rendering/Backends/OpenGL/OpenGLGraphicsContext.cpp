#include "OpenGLGraphicsContext.h"

#include "Nova/Core/App/App.h"

#include "glad/glad.h"

namespace Nova::Rendering
{
	OpenGLGraphicsContext::OpenGLGraphicsContext(uint32_t width, uint32_t height)
		: m_Width(width), m_Height(height)
	{}

	void OpenGLGraphicsContext::MakeCurrent()
	{
		// Call the function that actually will make this context current
		m_MakeCurrentDelegate();
	}

	
	void OpenGLGraphicsContext::SwapBuffers()
	{
		// Call the function that actually will swap this context's buffers
		m_SwapBuffersDelegate();
	}

	void OpenGLGraphicsContext::LoadExtensions(void* procFunc)
	{
		// Load extensions via glad
		int gladStatus = gladLoadGLLoader((GLADloadproc)procFunc);
		if (gladStatus == 0)
		{
			throw Exception("Failed to initialize Glad");
		}

		// Print out some OpenGL info
		const char* vendorStr = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
		const char* versionStr = reinterpret_cast<const char*>(glGetString(GL_VERSION));
		App::LogCore(LogLevel::Info, "Initialized OpenGL context - Vendor: {0}, Version: {1}", vendorStr, versionStr);
	}

	void OpenGLGraphicsContext::SetMakeCurrentCallback(std::function<void()> func)
	{
		m_MakeCurrentDelegate = func;
	}

	void OpenGLGraphicsContext::SetSwapBuffersCallback(std::function<void()> func)
	{
		m_SwapBuffersDelegate = func;
	}
}	
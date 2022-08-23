#include "OpenGLGraphicsContext.h"

#include "Nova/Core/App/App.h"

#include "Vendor/glad/include/glad/glad.h"

namespace Nova::Rendering
{
	void OpenGLGraphicsContext::MakeCurrent()
	{
		// Call the MakeCurrentDelegate function that actually will make this context current
		m_MakeCurrentDelegate();
	}

	void OpenGLGraphicsContext::SetMakeCurrentCallback(MakeCurrentDelegate func)
	{
		m_MakeCurrentDelegate = func;
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
}	
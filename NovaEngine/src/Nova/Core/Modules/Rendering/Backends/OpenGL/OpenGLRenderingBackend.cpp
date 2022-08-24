#include "OpenGLRenderingBackend.h"

#include "Nova/Core/App/App.h"

#include "glad/glad.h"

namespace Nova::Rendering
{
	OpenGLRenderingBackend::OpenGLRenderingBackend()
	{
		App::LogCore(LogLevel::Verbose, "Created OpenGL rendering backend");
	}

	OpenGLRenderingBackend::~OpenGLRenderingBackend()
	{
		App::LogCore(LogLevel::Verbose, "Destroyed OpenGL rendering backend");
	}

	void OpenGLRenderingBackend::InitGraphicsContext(Ref<GraphicsContext> context)
	{
		m_GraphicsContexts.push_back(context);
		LoadExtensionsForGraphicsContext(context);
	}

	List<Ref<GraphicsContext>> OpenGLRenderingBackend::GetActiveGraphicsContexts()
	{
		List<Ref<GraphicsContext>> contexts;

		auto it = m_GraphicsContexts.begin();

		while(it != m_GraphicsContexts.end())
		{
			auto lockedContext = (*it).lock();

			if (!lockedContext)
			{
				it = m_GraphicsContexts.erase(it);
			}
			else
			{
				contexts.push_back(lockedContext);
				it++;
			}
		}

		return contexts;
	}

	void OpenGLRenderingBackend::LoadExtensionsForGraphicsContext(Ref<GraphicsContext> context)
	{
		context->MakeCurrent();

		void* procFunc = context->GetExtensionsFunction();

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
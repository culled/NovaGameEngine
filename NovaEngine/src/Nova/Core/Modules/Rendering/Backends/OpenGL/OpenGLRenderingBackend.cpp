#include "OpenGLRenderingBackend.h"

#include "Nova/Core/App/App.h"

#include <glad/glad.h>

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

		// Only load extensions for contexts that are brand new (not shared)
		if(!context->GetIsShared())
			LoadExtensionsForGraphicsContext(context);
	}

	List<Ref<GraphicsContext>> OpenGLRenderingBackend::GetGraphicsContexts()
	{
		List<Ref<GraphicsContext>> contexts;

		// Make a list of valid contexts and erase invalid ones
		auto it = m_GraphicsContexts.begin();
		while (it != m_GraphicsContexts.end())
		{
			auto context = (*it).lock();

			if (context)
			{
				contexts.push_back(context);
				it++;
			}
			else
			{
				it = m_GraphicsContexts.erase(it);
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

		context->ReleaseCurrent();

		App::LogCore(LogLevel::Info, "Initialized OpenGL context - Vendor: {0}, Version: {1}", vendorStr, versionStr);
	}
}
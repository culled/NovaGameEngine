#include "OpenGLRenderingBackend.h"

#include "Nova/Core/App/App.h"
#include "OpenGLGraphicsContext.h"

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

	Ref<GraphicsContext> OpenGLRenderingBackend::CreateGraphicsContext(uint32_t width, uint32_t height)
	{
		auto context = MakeRef<OpenGLGraphicsContext>(width, height);
		m_GraphicsContexts.push_back(context);
		return context;
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
}
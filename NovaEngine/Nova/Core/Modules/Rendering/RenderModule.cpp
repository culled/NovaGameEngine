#include "RenderModule.h"

#include "Nova/Core/App/App.h"
#include "Backends/OpenGL/OpenGLRenderingBackend.h"

namespace Nova::Rendering
{
	RenderModule::RenderModule(int tickOrderOffset, RenderingBackendAPI backendAPI) :
		AppModule(tickOrderOffset)
	{
		if (s_Instance.lock())
		{
			// Can't have more than one render module!
			throw AppModuleInitException("A RenderModule is already in use");
		}

		CreateBackend(backendAPI);
	}

	// RefCounted ----------
	void RenderModule::Init()
	{
		s_Instance = GetSelfWeakRef<RenderModule>();
		App::LogCore(LogLevel::Verbose, "RenderModule created successfully");
	}

	// RefCounted ----------

	WeakRef<RenderModule> RenderModule::s_Instance;

	void RenderModule::CreateBackend(RenderingBackendAPI backendAPI)
	{
		switch (backendAPI)
		{
		case RenderingBackendAPI::OpenGL:
			m_RenderingBackend = MakeRef<OpenGLRenderingBackend>();
			break;
		default:
			throw Exception("Unsupported backend api");
		}
	}
}
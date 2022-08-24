#include "RenderModule.h"

#include "Nova/Core/App/App.h"
#include "Backends/OpenGL/OpenGLRenderingBackend.h"

namespace Nova::Rendering
{
	RenderModule::RenderModule(int tickOrderOffset, RenderingBackendAPI backendAPI) :
		AppModule(tickOrderOffset)
	{
		SetListenForTicks(true);

		if (s_Instance.lock())
		{
			// Can't have more than one render module!
			throw AppModuleInitException("A RenderModule is already in use");
		}

		App::LogCore(LogLevel::Verbose, "********** Initializing RenderModule... *********");

		CreateBackend(backendAPI);
	}

	RenderModule::~RenderModule()
	{
		App::LogCore(LogLevel::Verbose, "********** RenderModule destroyed *********");
	}

	// RefCounted ----------
	void RenderModule::Init()
	{
		s_Instance = GetSelfWeakRef<RenderModule>();

		App::LogCore(LogLevel::Verbose, "********** RenderModule initialized successfully *********");
	}

	// RefCounted ----------

	// TickListener ----------
	void RenderModule::Tick(double deltaTime)
	{
		auto contexts = m_RenderingBackend->GetActiveGraphicsContexts();

		for (const auto& context : contexts)
		{
			context->MakeCurrent();
			m_RenderLayerStack.BeginFrame(context, deltaTime);
			m_RenderLayerStack.EndFrame(context, deltaTime);
			context->SwapBuffers();
		}
	}

	// TickListener ----------

	WeakRef<RenderModule> RenderModule::s_Instance;

	void RenderModule::AppendRenderLayer(Ref<RenderLayer> layer)
	{
		m_RenderLayerStack.AppendLayer(layer);
	}

	void RenderModule::RemoveRenderLayer(Ref<RenderLayer> layer)
	{
		m_RenderLayerStack.RemoveLayer(layer);
	}

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
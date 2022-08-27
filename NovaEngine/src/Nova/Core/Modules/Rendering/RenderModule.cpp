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

		App::LogCore(LogLevel::Verbose, "********** Initializing RenderModule... *********");

		SetListenForTicks(true);
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
		m_RenderLayerStack.BeginFrame(deltaTime);

		auto contexts = m_RenderingBackend->GetGraphicsContexts();

		// Render for each context
		for (const auto& context : contexts)
		{
			context->MakeCurrent();

			m_RenderLayerStack.RenderContext(context);

			context->ReleaseCurrent();
		}

		// Swap all our context buffers
		for (const auto& context : contexts)
		{
			context->MakeCurrent();
			context->SwapBuffers();
			context->ReleaseCurrent();
		}

		m_RenderLayerStack.EndFrame();
	}

	// TickListener ----------

	WeakRef<RenderModule> RenderModule::s_Instance;

	void RenderModule::AddRenderLayer(Ref<RenderLayer> layer)
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
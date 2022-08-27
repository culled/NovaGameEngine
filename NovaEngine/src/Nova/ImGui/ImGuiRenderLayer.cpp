#include "ImGuiRenderLayer.h"

#include "Nova/Core/App/App.h"
#include "Nova/Core/Modules/Rendering/RenderModule.h"
#include "Nova/Core/Modules/Windowing/WindowingModule.h"

#include "renderers/ImGuiOpenGLRenderer.h"

#include <imgui.h>

namespace Nova::ImGui
{
	ImGuiRenderLayer::ImGuiRenderLayer()
	{
		CreateRenderer();

		App::LogCore(LogLevel::Verbose, "ImGui layer created");
	}

	ImGuiRenderLayer::~ImGuiRenderLayer()
	{
		m_Renderer.release();

		App::LogCore(LogLevel::Verbose, "ImGui layer destroyed");
	}

	// RefCounted ----------
	void ImGuiRenderLayer::Init()
	{
		Windowing::WindowingModule::Get()->OnWindowDestroyed.Connect(GetSelfRef<ImGuiRenderLayer>(), &ImGuiRenderLayer::WindowDestroyedCallback);
	}
	// RefCounted ----------

	void ImGuiRenderLayer::BeginFrame(double deltaTime)
	{
		m_DeltaTime = deltaTime;
	}

	void ImGuiRenderLayer::RenderContext(Ref<Rendering::GraphicsContext> context)
	{
		ImGuiContext* imGuiCtx = GetImGuiContextForGraphicsContext(context);

		// Don't continue if there's no ImGui context for this context
		if (!imGuiCtx)
			return;

		::ImGui::SetCurrentContext(imGuiCtx);

		// Begin a new frame in our renderer
		m_Renderer->BeginFrame(context, m_DeltaTime);
		::ImGui::NewFrame();

		context->RenderForLayer(GetID());

		::ImGui::Render();
		m_Renderer->RenderImGuiData();

		ImGuiIO& io = ::ImGui::GetIO();

		// Update and Render additional Platform Windows
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{

			::ImGui::UpdatePlatformWindows();
			::ImGui::RenderPlatformWindowsDefault();

			// ImGui may have made a different context current, so reset the current one here
			context->MakeCurrent();
		}
	}

	void ImGuiRenderLayer::EndFrame()
	{ }

	void ImGuiRenderLayer::CreateContextForWindow(Ref<Windowing::Window> window)
	{
		// Init ImGui
		::IMGUI_CHECKVERSION();
		ImGuiContext* context = ::ImGui::CreateContext();
		::ImGui::SetCurrentContext(context);

		// Dark theming
		::ImGui::StyleColorsDark();

		ImGuiStyle& style = ::ImGui::GetStyle();
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;

		ImGuiIO& io = ::ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

		m_WindowContextMap[window] = context;

		m_Renderer->InitializeNewImGuiContext(window);
	}

	void ImGuiRenderLayer::CreateRenderer()
	{
		using namespace Rendering;

		switch (RenderModule::Get()->GetBackend()->GetAPI())
		{
		case RenderingBackendAPI::OpenGL:
			m_Renderer = MakeExclusive<ImGuiOpenGLRenderer>();
			break;
		default:
			// TODO: better error handling
			throw Exception("Unsupported rendering API");
		}
	}

	ImGuiContext* ImGuiRenderLayer::GetImGuiContextForGraphicsContext(Ref<Rendering::GraphicsContext> context) const
	{
		for (auto it = m_WindowContextMap.begin(); it != m_WindowContextMap.end(); it++)
		{
			if (it->first->GetGraphicsContext().get() == context.get())
			{
				return it->second;
			}
		}

		return nullptr;
	}

	void ImGuiRenderLayer::WindowDestroyedCallback(Windowing::WindowDestroyedEvent& e)
	{
		auto it = m_WindowContextMap.find(e.Window);

		if (it == m_WindowContextMap.end())
			return;

		::ImGui::SetCurrentContext(it->second);

		m_Renderer->ShutdownImGuiContext(e.Window);

		// Cleanup this window's context
		::ImGui::DestroyContext(it->second);

		m_WindowContextMap.erase(it);
	}
}
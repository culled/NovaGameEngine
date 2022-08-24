#include "ImGuiRenderLayer.h"

#include "Nova/Core/App/App.h"
#include "Nova/Core/Modules/Rendering/RenderModule.h"

#include "renderers/ImGuiOpenGLRenderer.h"

#include "imgui.h"

namespace Nova::ImGui
{
	ImGuiRenderLayer::ImGuiRenderLayer()
	{
		// Init ImGui
		::IMGUI_CHECKVERSION();
		::ImGui::CreateContext();

		// Dark theming
		::ImGui::StyleColorsDark();

		ImGuiStyle& style = ::ImGui::GetStyle();
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;

		CreateRenderer();

		App::LogCore(LogLevel::Verbose, "ImGui layer initialized");
	}

	ImGuiRenderLayer::~ImGuiRenderLayer()
	{
		m_Renderer.release();
		::ImGui::DestroyContext();
		App::LogCore(LogLevel::Verbose, "ImGui layer destroyed");
	}

	void ImGuiRenderLayer::BeginFrame(Ref<Rendering::GraphicsContext> currentContext, double deltaTime)
	{
		// Resize ImGui to fit the current graphics context
		ImGuiIO& io = ::ImGui::GetIO();
		io.DisplaySize = ImVec2((float)currentContext->GetWidth(), (float)currentContext->GetHeight());
		io.DeltaTime = (float)deltaTime;

		// TODO: update mouse data & cursor, update gamepads (check imgui_impl_glfw)

		// Begin a new frame in ImGui
		m_Renderer->BeginFrame();
		::ImGui::NewFrame();

		// Tell all our listeners to do stuff related to ImGui now
		ImGuiRenderEvent e(App::GetDeltaTime());
		OnBeginFrame.Emit(e);
	}

	void ImGuiRenderLayer::EndFrame(Ref<Rendering::GraphicsContext> currentContext, double deltaTime)
	{
		// Have ImGui render the frame
		::ImGui::Render();
		m_Renderer->EndFrame();
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
}
#pragma once

#include "Nova/Core/EngineAPI.h"
#include "Nova/Core/Types/RefCounted.h"

namespace Nova::Rendering
{
	class GraphicsContext;
}

namespace Nova::Windowing
{
	class Window;
}

NovaAPI bool ImGui_ImplNova_Init(Nova::Ref<Nova::Windowing::Window> mainWindow);
NovaAPI void ImGui_ImplNova_Shutdown();
NovaAPI void ImGui_ImplNova_NewFrame(Nova::Ref<Nova::Rendering::GraphicsContext> context, double deltaTime);

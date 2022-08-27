#include "imgui_impl_nova.h"

#include "Nova/Core/Modules/Rendering/RenderingBackend.h"
#include "Nova/Core/Modules/Windowing/WindowingModule.h"
#include "Nova/Core/Modules/Windowing/Window.h"
#include "Nova/Core/Modules/Windowing/Monitor.h"
#include "Nova/Core/Modules/Rendering/GraphicsContext.h"

#include <imgui.h>

struct ImGui_ImplNova_Data
{
    Nova::WeakRef<Nova::Windowing::Window> MainWindow;
    double Time;
    ImVec2 LastValidMousePos;
    bool WantUpdateMonitors;

    ImGui_ImplNova_Data() { memset((void*)this, 0, sizeof(*this)); }
};

// Backend data stored in io.BackendPlatformUserData to allow support for multiple Dear ImGui contexts
static ImGui_ImplNova_Data* ImGui_ImplNova_GetBackendData()
{
    return ImGui::GetCurrentContext() ? (ImGui_ImplNova_Data*)ImGui::GetIO().BackendPlatformUserData : NULL;
}

// Forward Declarations
static void ImGui_ImplNova_UpdateMonitors();
static void ImGui_ImplNova_InitPlatformInterface();
static void ImGui_ImplNova_ShutdownPlatformInterface();

// Functions
static const char* ImGui_ImplNova_GetClipboardText(void* user_data)
{
    // TODO
    return nullptr;
}

static void ImGui_ImplNova_SetClipboardText(void* user_data, const char* text)
{
    // TODO
}

static ImGuiKey ImGui_ImplNova_KeyToImGuiKey(int key)
{
    // TODO
    return ImGuiKey_None;

    //switch (key)
    //{
    //    default: return ImGuiKey_None;
    //}
}

static int ImGui_ImplNova_KeyToModifier(int key)
{
    // TODO
    return 0;
}

static void ImGui_ImplNova_UpdateKeyModifiers(int mods)
{
    // TODO
}

bool ImGui_ImplNova_Init(Nova::Ref<Nova::Windowing::Window> mainWindow)
{
    ImGuiIO& io = ImGui::GetIO();
    IM_ASSERT(io.BackendPlatformUserData == NULL && "Already initialized a platform backend!");

    // Setup backend capabilities flags
    ImGui_ImplNova_Data* bd = IM_NEW(ImGui_ImplNova_Data)();
    io.BackendPlatformUserData = (void*)bd;
    io.BackendPlatformName = "imgui_impl_nova";
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;         // We can honor GetMouseCursor() values (optional)
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;          // We can honor io.WantSetMousePos requests (optional, rarely used)
    io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;    // We can create multi-viewports on the Platform side (optional)
#if GLFW_HAS_MOUSE_PASSTHROUGH || (GLFW_HAS_WINDOW_HOVERED && defined(_WIN32))
    io.BackendFlags |= ImGuiBackendFlags_HasMouseHoveredViewport; // We can call io.AddMouseViewportEvent() with correct data (optional)
#endif

    bd->MainWindow = mainWindow;
    bd->Time = 0.0;
    bd->WantUpdateMonitors = true;

    io.SetClipboardTextFn = ImGui_ImplNova_SetClipboardText;
    io.GetClipboardTextFn = ImGui_ImplNova_GetClipboardText;
    io.ClipboardUserData = nullptr; // TODO

    // Update monitors the first time
    ImGui_ImplNova_UpdateMonitors();

    // Our mouse update function expect PlatformHandle to be filled for the main viewport
    ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    main_viewport->PlatformHandle = mainWindow->GetBackendWindowHandle();
    main_viewport->PlatformHandleRaw = mainWindow->GetPlatformWindowHandle();

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        ImGui_ImplNova_InitPlatformInterface();

    return true;
}

void ImGui_ImplNova_Shutdown()
{
    ImGui_ImplNova_Data* bd = ImGui_ImplNova_GetBackendData();
    IM_ASSERT(bd != NULL && "No platform backend to shutdown, or already shutdown?");
    ImGuiIO& io = ImGui::GetIO();

    ImGui_ImplNova_ShutdownPlatformInterface();

    io.BackendPlatformName = NULL;
    io.BackendPlatformUserData = NULL;
    IM_DELETE(bd);
}

static void ImGui_ImplNova_UpdateMouseData()
{
    // TODO
}

static void ImGui_ImplNova_UpdateMouseCursor()
{
    // TODO
}

// Update gamepad inputs
static inline float Saturate(float v) { return v < 0.0f ? 0.0f : v  > 1.0f ? 1.0f : v; }
static void ImGui_ImplNova_UpdateGamepads()
{
    // TODO
}

static void ImGui_ImplNova_UpdateMonitors()
{
    ImGui_ImplNova_Data* bd = ImGui_ImplNova_GetBackendData();
    ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();

    Nova::List<Nova::Ref<Nova::Windowing::Monitor>> monitors = Nova::Windowing::WindowingModule::Get()->GetMonitors();

    platform_io.Monitors.resize(0);

    for (int n = 0; n < monitors.size(); n++)
    {
        ImGuiPlatformMonitor monitor;
        Nova::Vector2i monitorPosition = monitors[n]->GetPosition();
        monitor.MainPos = ImVec2((float)monitorPosition.X, (float)monitorPosition.Y);

        Nova::Windowing::MonitorVideoMode videoMode = monitors[n]->GetCurrentVideoMode();
        monitor.MainSize = ImVec2((float)videoMode.Size.X, (float)videoMode.Size.Y);

        Nova::Recti workArea = monitors[n]->GetWorkArea();
        monitor.WorkPos = ImVec2((float)workArea.Position.X, (float)workArea.Position.Y);
        monitor.WorkSize = ImVec2((float)workArea.Size.X, (float)workArea.Size.Y);

        monitor.DpiScale = (float)monitors[n]->GetDpiScale().X;

        platform_io.Monitors.push_back(monitor);
    }

    bd->WantUpdateMonitors = false;
}

void ImGui_ImplNova_NewFrame(Nova::Ref<Nova::Rendering::GraphicsContext> context, double deltaTime)
{
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplNova_Data* bd = ImGui_ImplNova_GetBackendData();
    IM_ASSERT(bd != NULL && "Did you call ImGui_ImplNova_Init()?");

    if (bd->WantUpdateMonitors)
        ImGui_ImplNova_UpdateMonitors();

    // Setup display size (every frame to accommodate for window resizing)
    Nova::Vector2i size = context->GetFramebufferSize();
    io.DisplaySize = ImVec2((float)size.X, (float)size.Y);

    // Setup time step
    io.DeltaTime = (float)deltaTime;
    bd->Time = bd->Time + deltaTime;

    ImGui_ImplNova_UpdateMouseData();
    ImGui_ImplNova_UpdateMouseCursor();

    // Update game controllers (if enabled and available)
    ImGui_ImplNova_UpdateGamepads();
}

//--------------------------------------------------------------------------------------------------------
// MULTI-VIEWPORT / PLATFORM INTERFACE SUPPORT
// This is an _advanced_ and _optional_ feature, allowing the backend to create and handle multiple viewports simultaneously.
// If you are new to dear imgui or creating a new binding for dear imgui, it is recommended that you completely ignore this section first..
//--------------------------------------------------------------------------------------------------------

// Helper structure we store in the void* RenderUserData field of each ImGuiViewport to easily retrieve our backend data.
struct ImGui_ImplNova_ViewportData
{
    Nova::WeakRef<Nova::Windowing::Window> Window;
    int IgnoreWindowPosEventFrame;
    int IgnoreWindowSizeEventFrame;

    ImGui_ImplNova_ViewportData() { IgnoreWindowSizeEventFrame = IgnoreWindowPosEventFrame = -1; }
    ~ImGui_ImplNova_ViewportData() {}
};

static void ImGui_ImplNova_CreateWindow(ImGuiViewport* viewport)
{
    ImGui_ImplNova_Data* bd = ImGui_ImplNova_GetBackendData();
    ImGui_ImplNova_ViewportData* vd = IM_NEW(ImGui_ImplNova_ViewportData)();
    viewport->PlatformUserData = vd;

    Nova::Windowing::WindowCreateParams createParams(Nova::Vector2i((int)viewport->Size.x, (int)viewport->Size.y), "No Title Yet");
    createParams.InitialPosition = Nova::Vector2i((int)viewport->Pos.x, (int)viewport->Pos.y);
    createParams.ShareGfxContext = bd->MainWindow.lock()->GetGraphicsContext();
    createParams.VSync = false;

    auto window = Nova::Windowing::WindowingModule::Get()->CreateWindow(createParams).lock();
    vd->Window = window;

    viewport->PlatformHandle = window->GetBackendWindowHandle();
    viewport->PlatformHandleRaw = window->GetPlatformWindowHandle();
}

static void ImGui_ImplNova_DestroyWindow(ImGuiViewport* viewport)
{
    ImGui_ImplNova_Data* bd = ImGui_ImplNova_GetBackendData();

    if (ImGui_ImplNova_ViewportData* vd = (ImGui_ImplNova_ViewportData*)viewport->PlatformUserData)
    {
        Nova::Windowing::WindowingModule::Get()->DestroyWindow(vd->Window.lock());
        IM_DELETE(vd);
    }

    viewport->PlatformUserData = viewport->PlatformHandle = NULL;
}

static void ImGui_ImplNova_ShowWindow(ImGuiViewport* viewport)
{
    ImGui_ImplNova_ViewportData* vd = (ImGui_ImplNova_ViewportData*)viewport->PlatformUserData;

    if (auto windowPtr = vd->Window.lock())
        windowPtr->Show();
}

static ImVec2 ImGui_ImplNova_GetWindowPos(ImGuiViewport* viewport)
{
    ImGui_ImplNova_ViewportData* vd = (ImGui_ImplNova_ViewportData*)viewport->PlatformUserData;

    Nova::Vector2i pos;

    if (auto windowPtr = vd->Window.lock())
         pos = windowPtr->GetPosition();

    return ImVec2((float)pos.X, (float)pos.Y);
}

static void ImGui_ImplNova_SetWindowPos(ImGuiViewport* viewport, ImVec2 pos)
{
    ImGui_ImplNova_ViewportData* vd = (ImGui_ImplNova_ViewportData*)viewport->PlatformUserData;

    if (auto windowPtr = vd->Window.lock())
    {
        vd->IgnoreWindowPosEventFrame = ImGui::GetFrameCount();
        windowPtr->SetPosition(Nova::Vector2i((int)pos.x, (int)pos.y));
    }
}

static ImVec2 ImGui_ImplNova_GetWindowSize(ImGuiViewport* viewport)
{
    ImGui_ImplNova_ViewportData* vd = (ImGui_ImplNova_ViewportData*)viewport->PlatformUserData;

    Nova::Vector2i size;
    if (auto windowPtr = vd->Window.lock())
        size = windowPtr->GetSize();

    return ImVec2((float)size.X, (float)size.Y);
}

static void ImGui_ImplNova_SetWindowSize(ImGuiViewport* viewport, ImVec2 size)
{
    ImGui_ImplNova_ViewportData* vd = (ImGui_ImplNova_ViewportData*)viewport->PlatformUserData;

    if (auto windowPtr = vd->Window.lock())
    {
        vd->IgnoreWindowSizeEventFrame = ImGui::GetFrameCount();
        windowPtr->SetSize(Nova::Vector2i((int)size.x, (int)size.y));
    }
}

static void ImGui_ImplNova_SetWindowTitle(ImGuiViewport* viewport, const char* title)
{
    ImGui_ImplNova_ViewportData* vd = (ImGui_ImplNova_ViewportData*)viewport->PlatformUserData;

    if (auto windowPtr = vd->Window.lock())
        windowPtr->SetTitle(title);
}

static void ImGui_ImplNova_SetWindowFocus(ImGuiViewport* viewport)
{
    ImGui_ImplNova_ViewportData* vd = (ImGui_ImplNova_ViewportData*)viewport->PlatformUserData;

    if (auto windowPtr = vd->Window.lock())
        windowPtr->Focus();
}

static bool ImGui_ImplNova_GetWindowFocus(ImGuiViewport* viewport)
{
    ImGui_ImplNova_ViewportData* vd = (ImGui_ImplNova_ViewportData*)viewport->PlatformUserData;

    if (auto windowPtr = vd->Window.lock())
        return windowPtr->GetIsFocused();

    return false;
}

static bool ImGui_ImplNova_GetWindowMinimized(ImGuiViewport* viewport)
{
    ImGui_ImplNova_ViewportData* vd = (ImGui_ImplNova_ViewportData*)viewport->PlatformUserData;

    if(auto windowPtr = vd->Window.lock())
        return windowPtr->GetIsMinimized();

    return false;
}

static void ImGui_ImplNova_SetWindowAlpha(ImGuiViewport* viewport, float alpha)
{
    ImGui_ImplNova_ViewportData* vd = (ImGui_ImplNova_ViewportData*)viewport->PlatformUserData;

    if (auto windowPtr = vd->Window.lock())
        windowPtr->SetOpacity((double)alpha);
}

static void ImGui_ImplNova_RenderWindow(ImGuiViewport* viewport, void*)
{
    ImGui_ImplNova_Data* bd = ImGui_ImplNova_GetBackendData();
    ImGui_ImplNova_ViewportData* vd = (ImGui_ImplNova_ViewportData*)viewport->PlatformUserData;

    if (auto windowPtr = vd->Window.lock())
        windowPtr->GetGraphicsContext()->MakeCurrent();
}

// Buffer swapping is handled by Nova, so no need to do it here
static void ImGui_ImplNova_SwapBuffers(ImGuiViewport* viewport, void*)
{
    //ImGui_ImplNova_Data* bd = ImGui_ImplNova_GetBackendData();
    //ImGui_ImplNova_ViewportData* vd = (ImGui_ImplNova_ViewportData*)viewport->PlatformUserData;
    //
    //if (auto windowPtr = vd->Window.lock())
    //{
    //    auto context = windowPtr->GetGraphicsContext();
    //    context->MakeCurrent();
    //    context->SwapBuffers();
    //    context->ReleaseCurrent();
    //}
}

static void ImGui_ImplNova_InitPlatformInterface()
{
    // Register platform interface (will be coupled with a renderer interface)
    ImGui_ImplNova_Data* bd = ImGui_ImplNova_GetBackendData();
    ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();
    platform_io.Platform_CreateWindow = ImGui_ImplNova_CreateWindow;
    platform_io.Platform_DestroyWindow = ImGui_ImplNova_DestroyWindow;
    platform_io.Platform_ShowWindow = ImGui_ImplNova_ShowWindow;
    platform_io.Platform_SetWindowPos = ImGui_ImplNova_SetWindowPos;
    platform_io.Platform_GetWindowPos = ImGui_ImplNova_GetWindowPos;
    platform_io.Platform_SetWindowSize = ImGui_ImplNova_SetWindowSize;
    platform_io.Platform_GetWindowSize = ImGui_ImplNova_GetWindowSize;
    platform_io.Platform_SetWindowFocus = ImGui_ImplNova_SetWindowFocus;
    platform_io.Platform_GetWindowFocus = ImGui_ImplNova_GetWindowFocus;
    platform_io.Platform_GetWindowMinimized = ImGui_ImplNova_GetWindowMinimized;
    platform_io.Platform_SetWindowTitle = ImGui_ImplNova_SetWindowTitle;
    platform_io.Platform_RenderWindow = ImGui_ImplNova_RenderWindow;
    platform_io.Platform_SwapBuffers = ImGui_ImplNova_SwapBuffers;
    platform_io.Platform_SetWindowAlpha = ImGui_ImplNova_SetWindowAlpha;
//#if GLFW_HAS_VULKAN
//    platform_io.Platform_CreateVkSurface = ImGui_ImplNova_CreateVkSurface;
//#endif

    // Register main window handle (which is owned by the main application, not by us)
    // This is mostly for simplicity and consistency, so that our code (e.g. mouse handling etc.) can use same logic for main and secondary viewports.
    ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui_ImplNova_ViewportData* vd = IM_NEW(ImGui_ImplNova_ViewportData)();
    vd->Window = bd->MainWindow;
    main_viewport->PlatformUserData = vd;
    main_viewport->PlatformHandle = bd->MainWindow.lock()->GetBackendWindowHandle();
}

static void ImGui_ImplNova_ShutdownPlatformInterface()
{
    ImGui::DestroyPlatformWindows();
}
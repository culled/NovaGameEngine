project "ImGui"
    location "../../ImGui"
    kind "StaticLib"
    language "C++"
    staticruntime "on"

    targetdir "%{OutputDir.bin}/%{prj.name}"
    objdir "%{OutputDir.obj}/%{prj.name}"

    files
    {
        "../../ImGui/imconfig.h",
        "../../ImGui/imgui.cpp",
        "../../ImGui/imgui.h",
        "../../ImGui/imgui_demo.cpp",
        "../../ImGui/imgui_draw.cpp",
        "../../ImGui/imgui_internal.h",
        "../../ImGui/imgui_tables.cpp",
        "../../ImGui/imgui_widgets.cpp",
        "../../ImGui/imstb_rectpack.h",
        "../../ImGui/imstb_textedit.h",
        "../../ImGui/imstb_truetype.h",
        "../../ImGui/backends/imgui_impl_opengl3.h",
        "../../ImGui/backends/imgui_impl_opengl3.cpp",
    }

    includedirs
    {
        "%{IncludeDir.ImGui}",
    }

    defines
    {
        "IMGUI_IMPL_OPENGL_LOADER_GLAD",
    }

    filter "system:windows"
        systemversion "latest"
    
    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
        
    filter "configurations:Release"
        runtime "Release"
        optimize "on"
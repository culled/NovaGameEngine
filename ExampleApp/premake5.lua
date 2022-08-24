project "ExampleApp"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir "%{OutputDir.bin}/%{prj.name}"
    objdir "%{OutputDir.obj}/%{prj.name}"

    files
    {
        "App/**.h",
        "App/**.cpp",
    }

    includedirs
    {
        "%{IncludeDir.nova}",
        "%{IncludeDir.ImGui}",
    }

    links
    {
        "NovaEngine",
        "ImGui",
    }

    filter "system:windows"
        links
        {
            "opengl32.lib"
        }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
        
    filter "configurations:Release"
        runtime "Release"
        optimize "on"
project "ExampleApp"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir "%{OutputDir.bin}%{prj.name}"
    objdir "%{OutputDir.obj}%{prj.name}"

    files
    {
        "App\\**.h",
        "App\\**.cpp",
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
        "SDL2.lib",
    }

    libdirs
    {
        "%{LibraryDir.sdl}",
    }

    filter "system:windows"
        postbuildcommands
        {
            "xcopy /D /Y ..\\NovaEngine\\vendor\\SDL2\\lib\\x64\\SDL2.dll ..\\bin\\" .. outputdir .. "%{prj.name}\\SDL2.dll"
        }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
        
    filter "configurations:Release"
        runtime "Release"
        optimize "on"
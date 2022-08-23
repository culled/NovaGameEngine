project "GLAD"
    location "../../glad"
    kind "StaticLib"
    language "C"
    staticruntime "on"

    targetdir "%{OutputDir.bin}/%{prj.name}"
    objdir "%{OutputDir.obj}/%{prj.name}"

    files
    {
        "../../glad/**.h",
        "../../glad/**.c",
    }

    includedirs
    {
        "%{IncludeDir.glad}",
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
        
    filter "configurations:Release"
        runtime "Release"
        optimize "on"
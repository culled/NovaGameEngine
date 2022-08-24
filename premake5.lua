workspace "NovaGameEngine"
    configurations { "Debug", "Release" }
    architecture "x64"

    startproject "ExampleApp"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to the workspace
IncludeDir = {}
IncludeDir["nova"] = "%{wks.location}/NovaEngine/src"
IncludeDir["glad"] = "%{wks.location}/NovaEngine/vendor/glad/include"
IncludeDir["glfw"] = "%{wks.location}/NovaEngine/vendor/glfw/include"
IncludeDir["ImGui"] = "%{wks.location}/NovaEngine/vendor/ImGui"

OutputDir = {}
OutputDir["bin"] = "%{wks.location}/bin/" .. outputdir
OutputDir["obj"] = "%{wks.location}/obj/" .. outputdir

group "Dependencies"
    include "NovaEngine/vendor/premakes/glfw"
    include "NovaEngine/vendor/premakes/glad"
    include "NovaEngine/vendor/premakes/ImGui"
    
group ""
    include "NovaEngine"
    include "ExampleApp"
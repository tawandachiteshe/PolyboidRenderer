project "easy_profiler"
    kind "StaticLib"
    language "C++"
    staticruntime "off"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    files
    {
        "src/**"
    }
    
    includedirs
    {
        "include",
    }

    defines 
    {
        "EASY_PROFILER_VERSION_MAJOR=2",
		"EASY_PROFILER_VERSION_MINOR=1",
        "EASY_PROFILER_VERSION_PATCH=0",
        "BUILD_WITH_EASY_PROFILER=1"
    }


    filter "system:windows"
    systemversion "latest"
    
    filter "configurations:Debug"
    runtime "Debug"
    symbols "on"
    
    filter "configurations:Release"
    runtime "Release"
    optimize "on"
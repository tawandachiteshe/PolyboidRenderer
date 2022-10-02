project "imgui"
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
        "src",
        "%{wks.location}/Engine/Vendor/glfw/include",
    }
    
    filter "system:windows"
    systemversion "latest"
    
    filter "configurations:Debug"
    runtime "Debug"
    symbols "on"
    
    filter "configurations:Release"
    runtime "Release"
    optimize "on"
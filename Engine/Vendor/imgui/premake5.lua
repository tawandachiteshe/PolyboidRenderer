project "imgui"
    kind "StaticLib"
    language "C++"
    staticruntime "off"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    VULKAN_SDK = os.getenv("VULKAN_SDK")
    
    files
    {
        "src/**"
    }
    
    includedirs
    {
        "src",
        "%{wks.location}/Engine/Vendor/glfw/include",
        "%{wks.location}/Engine/Vendor/glad/include",
        "%{VULKAN_SDK}/Include",
    }
    
    filter "system:windows"
    systemversion "latest"
    
    filter "configurations:Debug"
    runtime "Debug"
    symbols "on"
    
    filter "configurations:Release"
    runtime "Release"
    optimize "on"
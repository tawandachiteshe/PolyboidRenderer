project "Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
	staticruntime "off"

    pchheader "boidpch.h"
    pchsource "src/boidpch.cpp"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    VULKAN_SDK = os.getenv("VULKAN_SDK")

    includedirs
    {
        "src",
        "Vendor/spdlog/include",
        "Vendor/msgpack/include",
        "Vendor/glfw/include",
        "Vendor/stb/include",
        "Vendor/glad/include",
        "Vendor/assimp/include",
        "Vendor/Optick/include",
        "Vendor/hashpp/include",
        "Vendor/mono/include",
        "Vendor/entt",
        "Vendor/imgui/src",
        "Vendor/json/include",
        "Vendor/siglot/include",
        "Vendor/taskflow",
        "Vendor/file_watcher",
        "%{VULKAN_SDK}/Include",
        "Vendor/yalantinglibs/include",
    }

    dependson
    {
        "PolyboidManaged"
    }

    defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

    links
    {
        "GLFW",
        "glad",
        "imgui",
        "Vendor/assimp/bin/assimp.lib",
        "Vendor/mono/lib/libmono-static-sgen.lib",
        "Ws2_32.lib",
        "Bcrypt.lib",
        "Winmm.lib",
        "Version.lib",
        "opengl32.lib"
    }

    files 
    { 
        "src/**.h", 
        "src/**.c",
        "src/**.cpp",
        "src/**.hpp"
    }

    filter "configurations:Debug"
       defines { "DEBUG" }
       symbols "On"

       links
       {
           "%{VULKAN_SDK}/Lib/shaderc_shared.lib",
           "%{VULKAN_SDK}/Lib/spirv-cross-cored.lib",
           "%{VULKAN_SDK}/Lib/spirv-cross-glsld.lib",
           "%{VULKAN_SDK}/Lib/glslangd.lib",
           "%{VULKAN_SDK}/Lib/SPIRV-Toolsd.lib",
           "%{VULKAN_SDK}/Lib/spirv-cross-reflectd.lib",
           "Vendor/Optick/lib/x64/debug/OptickCore.lib"
       }
 
    filter "configurations:Release"
       defines { "NDEBUG" }
       optimize "On"

       links
       {
           "%{VULKAN_SDK}/Lib/shaderc_shared.lib",
           "%{VULKAN_SDK}/Lib/spirv-cross-core.lib",
           "%{VULKAN_SDK}/Lib/spirv-cross-glsl.lib",
           "%{VULKAN_SDK}/Lib/glslang.lib",
           "%{VULKAN_SDK}/Lib/SPIRV-Tools.lib",
           "%{VULKAN_SDK}/Lib/spirv-cross-reflect.lib",
           "Vendor/Optick/lib/x64/Release/OptickCore.lib"
       }
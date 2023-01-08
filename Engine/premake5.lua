project "Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
	staticruntime "off"

    pchheader "boidpch.h"
    pchsource "src/boidpch.cpp"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")


    includedirs
    {
        "src",
        "Vendor/spdlog/include",
        "Vendor/glm",
        "Vendor/glfw/include",
        "Vendor/stb/include",
        "Vendor/glad/include",
        "Vendor/assimp/include",
        "Vendor/mono/include",
        "Vendor/spirv/include",
        "Vendor/entt",
        "Vendor/imgui/src",
    }

    dependson
    {
        "PolyboidManaged"
    }

    defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE",
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
        "src/**.hpp",
        "Vendor/glm/glm/**.hpp",
        "Vendor/glm/glm/**.inl",
    }

    filter "configurations:Debug"
       defines { "DEBUG" }
       symbols "On"
 
    filter "configurations:Release"
       defines { "NDEBUG" }
       optimize "On"

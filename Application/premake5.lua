
project "Application"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files { "src/**.h", "src/**.c", "src/**.cpp", "src/**.hpp", "Assets/**" }

    includedirs
    {

        "%{wks.location}/Engine/src",
        "%{wks.location}/Engine/Vendor/spdlog/include",
        "%{wks.location}/Engine/Vendor/glfw/include",
        "%{wks.location}/Engine/Vendor/glm",
        "%{wks.location}/Engine/Vendor/glad/include",
        "%{wks.location}/Engine/Vendor/imgui/src",
        "%{wks.location}/Engine/Vendor/entt",
        "%{wks.location}/Engine/Vendor/stb/include",

    }

    links 
    { 
        "Engine"
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
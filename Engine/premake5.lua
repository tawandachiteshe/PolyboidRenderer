project "Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
	staticruntime "off"

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
    }

    links
    {
        "GLFW",
        "glad",
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

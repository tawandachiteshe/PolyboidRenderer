
project "PolyEditor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files { "src/**.h", "src/**.c", "src/**.cpp", "src/**.hpp", "Assets/**" }
    VULKAN_SDK = os.getenv("VULKAN_SDK")

    includedirs
    {

        "%{wks.location}/Engine/src",
        "%{wks.location}/Engine/Vendor/spdlog/include",
        "%{wks.location}/Engine/Vendor/assimp/include",
        "%{wks.location}/Engine/Vendor/glfw/include",
        "%{wks.location}/Engine/Vendor/glad/include",
        "%{wks.location}/Engine/Vendor/imgui/src",
        "%{wks.location}/Engine/Vendor/entt",
        "%{wks.location}/Engine/Vendor/Optick/include",
        "%{wks.location}/Engine/Vendor/stb/include",
        "%{wks.location}/Engine/Vendor/json/include",
        "%{wks.location}/Engine/Vendor/spirv/include",
        "%{wks.location}/PolyEditor/src",
        "%{VULKAN_SDK}/Include"

    }

    dependson
    {
        "PolyboidManaged"
    }

    links 
    { 
        "Engine"
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
        postbuildcommands {
            "{COPY} %{wks.location}/Engine/Vendor/assimp/bin/assimp-vc143-mt.dll %{cfg.targetdir}",
            "{COPY} %{wks.location}/Engine/Vendor/Optick/lib/x64/debug/OptickCore.dll %{cfg.targetdir}",
            "{COPY} %{wks.location}/Engine/Vendor/shaderc/shaderc_shared.dll %{cfg.targetdir}"
        }

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
        postbuildcommands {
            "{COPY} %{wks.location}/Engine/Vendor/assimp/bin/assimp-vc143-mt.dll %{cfg.targetdir}",
            "{COPY} %{wks.location}/Engine/Vendor/Optick/lib/x64/release/OptickCore.dll %{cfg.targetdir}",
            "{COPY} %{wks.location}/Engine/Vendor/shaderc/shaderc_shared.dll %{cfg.targetdir}"
        }
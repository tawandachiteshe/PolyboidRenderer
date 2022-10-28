workspace "PolyboidRenderer"
    architecture "x86_64"
    configurations { "Debug", "Release" }
    
    startproject "PolyEditor"

    flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
include "Engine/Vendor/glfw"
include "Engine/Vendor/glad"
include "Engine/Vendor/imgui"
group ""

include "Engine"
include "PolyEditor"
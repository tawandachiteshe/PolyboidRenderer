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
include "Engine/Vendor/imgui"
include "Engine/Vendor/glfw"
group ""

group "Dev"
include "PolyboidManaged"
include "Engine"
include "PolyEditor"
group ""
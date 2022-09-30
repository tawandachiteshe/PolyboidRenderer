workspace "PolyboidRenderer"
    architecture "x86_64"
    configurations { "Debug", "Release" }
    
    startproject "Application"

    flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
include "Engine/Vendor/glfw"
include "Engine/Vendor/glad"
group ""

include "Engine"
include "Application"
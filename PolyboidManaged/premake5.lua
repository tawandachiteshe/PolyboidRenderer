project "PolyboidManaged"
    kind "SharedLib"
    language "C#"
    dotnetframework "4.7.2"

    targetdir ("../PolyEditor/Resources/Scripts")
	objdir ("../PolyEditor/Resources/Scripts/Intermediates")

    files 
	{
		"Source/**.cs",
        "Source/*.cs",
		"Properties/**.cs"
	}

    filter "configurations:Debug"
        optimize "Off"
        symbols "Default"

    filter "configurations:Release"
        optimize "On"
        symbols "Default"

    filter "configurations:Dist"
        optimize "Full"
        symbols "Off"
project "Lua"
	kind "StaticLib"
	language "C"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.c",
		"src/**.h"
	}

	includedirs
	{
		"src"
	}

	filter "system:linux"
		pic "On"
		systemversion "latest"
		staticruntime "On"

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "CM_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "CM_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "CM_DIST"
		runtime "Release"
		optimize "on"

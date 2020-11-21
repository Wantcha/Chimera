workspace "Chimera"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Chimera/vendor/GLFW/include"
IncludeDir["Glad"] = "Chimera/vendor/GLAD/include"
IncludeDir["ImGui"] = "Chimera/vendor/imgui"

-- Include premake files from other projects
include "Chimera/vendor/GLFW"
include "Chimera/vendor/Glad"
include "Chimera/vendor/imgui"

project "Chimera"
	location "Chimera"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "cmpch.h"
	pchsource "Chimera/src/cmpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"CM_PLATFORM_WINDOWS",
			"CM_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}
		
		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
		}

	filter "configurations:Debug"
		defines "CM_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "CM_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "CM_DIST"
		runtime "Release"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Chimera/vendor/spdlog/include",
		"Chimera/src"
	}

	links
	{
		"Chimera"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"CM_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "CM_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "CM_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "CM_DIST"
		runtime "Release"
		optimize "On"
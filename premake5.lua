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
IncludeDir["glm"] = "Chimera/vendor/glm"

-- Include premake files from other projects
include "Chimera/vendor/GLFW"
include "Chimera/vendor/Glad"
include "Chimera/vendor/imgui"

project "Chimera"
	location "Chimera"
	kind "StaticLib"
	language "C++"
	staticruntime "on"
	cppdialect "C++17"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "cmpch.h"
	pchsource "Chimera/src/cmpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"CM_PLATFORM_WINDOWS",
			"CM_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

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

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

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
		"Chimera/src",
		"Chimera/vendor",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Chimera"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"CM_PLATFORM_WINDOWS"
		}

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
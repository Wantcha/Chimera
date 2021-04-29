project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/release/")--("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/Chimera/vendor/spdlog/include",
		"%{wks.location}/Chimera/src",
		"%{wks.location}/Chimera/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.box2d}",
		"%{IncludeDir.Lua}",
		"%{IncludeDir.sol2}",
		"%{IncludeDir.yaml_cpp}"
	}

	links
	{
		"Chimera"
	}

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

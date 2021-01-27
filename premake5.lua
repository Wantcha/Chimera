--include "./vendor/premake/premake_customization/solution_items.lua"

workspace "Chimera"
	architecture "x86_64"
	startproject "Chimera-Editor"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Chimera/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Chimera/vendor/GLAD/include"
IncludeDir["ImGui"] = "%{wks.location}/Chimera/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/Chimera/vendor/glm"
IncludeDir["entt"] = "%{wks.location}/Chimera/vendor/entt/include"
IncludeDir["stb_image"] = "%{wks.location}/Chimera/vendor/stb_image"
IncludeDir["yaml_cpp"] = "%{wks.location}/Chimera/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/Chimera/vendor/ImGuizmo"

-- Include premake files from other projects
group "Dependencies"
	include "vendor/premake"
	include "Chimera/vendor/GLFW"
	include "Chimera/vendor/Glad"
	include "Chimera/vendor/imgui"
	include "Chimera/vendor/yaml-cpp"
group ""

include "Chimera"

include "Sandbox"

include "Chimera-Editor"
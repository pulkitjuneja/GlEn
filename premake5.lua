workspace "Glen"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Glen/ext/imgui"

project "Glen"
	location "Glen"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/Glen.h"
	}

	libdirs 
	{
		"%{prj.name}/ext/SFML/lib",
		"%{prj.name}/ext/Glew/lib",
		"%{prj.name}/ext/assimp/lib"
	}

	links
	{
		"opengl32.lib",
		"glew32.lib",
		"sfml-graphics.lib",
		"sfml-window.lib",
		"sfml-system.lib",
		"assimp.lib",
		"ImGui"
	}

	includedirs
	{
		"%{prj.name}/ext/glm",
		"%{prj.name}/ext/stbi",
		"%{prj.name}/ext/SFML/include",
		"%{prj.name}/ext/Glew/include",
		"%{prj.name}/ext/assimp/include",
		"%{prj.name}/ext/imgui"
	}

	filter "system:windows"
		cppdialect "C++14"
		staticruntime "On"
		systemversion "10.0.18362.0"

		defines
		{
			"GLN_PLATFORM_WINDOWS",
			"GLN_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

		buildoptions "/MDd"

	filter "configurations:Debug"
		defines "GLN_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "GLN_RELEASE"
		optimize "On"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name	}/src/**.cpp",
		"%{prj.name}/Sandbox.cpp"
	}

	includedirs
	{
		"Glen/ext/glm",
		"Glen",
		"Glen/src",
		"Glen/ext/SFML/include",
		"Glen/ext/Glew/include",
		"Glen/ext/assimp/include",
		"Glen/ext/stbi"
	}

	links
	{
		"Glen"
	}

	filter "system:windows"
		cppdialect "C++14"
		staticruntime "On"
		systemversion "10.0.18362.0"

		defines
		{
			"GLN_PLATFORM_WINDOWS"
		}

		
		buildoptions "/MDd"

	filter "configurations:Debug"
		defines "GLN_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "GLN_RELEASE"
		optimize "On"
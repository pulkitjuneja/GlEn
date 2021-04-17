workspace "Glen"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Glen/ext/imGui"
include "Glen/ext/glfw"

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
		"%{prj.name}/Glen.h",
		"%{prj.name}/ext/ImGui/examples/imgui_impl_glfw.cpp",
		"%{prj.name}/ext/ImGui/examples/imgui_impl_opengl3.cpp"
	}

	libdirs 
	{
		"%{prj.name}/ext/Glew/lib",
		"%{prj.name}/ext/assimp/lib",
		"%{prj.name}/ext/Physx/lib"
	}

	links
	{
		"opengl32.lib",
		"glew32.lib",
		"assimp.lib",
		"GLFW",
		"ImGui",
		"PhysXCooking_64.lib",
		"PhysX_64.lib",
		"PhysXCommon_64.lib",
		"PhysXFoundation_64.lib",
		"PhysXPvdSDK_static_64.lib",
		"PhysXExtensions_static_64.lib"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/ext/glm",
		"%{prj.name}/ext/stbi",
		"%{prj.name}/ext/Glew/include",
		"%{prj.name}/ext/assimp/include",
		"%{prj.name}/ext/imgui",
		"%{prj.name}/ext/glfw/glfw/include",
		"%{prj.name}/ext/Physx/Physx/pxshared/include",
		"%{prj.name}/ext/Physx/Physx/include"
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

	libdirs 
	{
		"Glen/ext/Glew/lib",
		"Glen/ext/assimp/lib",
		"Glen/ext/Physx/lib"
	}


	includedirs
	{
		"Glen/ext/glm",
		"Glen",
		"Glen/src",
		"Glen/ext/Glew/include",
		"Glen/ext/assimp/include",
		"Glen/ext/stbi",
		"Glen/ext/imgui",
		"Glen/ext/glfw/glfw/include",
		"Glen/ext/Physx/Physx/pxshared/include",
		"Glen/ext/Physx/Physx/include"
	}

	links
	{
		"Glen",
		"PhysXCooking_64.lib",
		"PhysX_64.lib",
		"PhysXCommon_64.lib",
		"PhysXFoundation_64.lib"
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
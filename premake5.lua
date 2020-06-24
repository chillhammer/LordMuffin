workspace "RaggedSkeleton"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = 		"Vendor/Lib/GLFW/include"
IncludeDir["GLAD"] = 		"Vendor/Lib/GLAD/include"
IncludeDir["ImGui"] =	 	"Vendor/Lib/imgui"
IncludeDir["glm"] = 		"Vendor/Lib/glm"
IncludeDir["spdlog"] = 		"Vendor/Lib/spdlog/include"
IncludeDir["Assimp"] = 		"Vendor/Lib/Assimp/include"
IncludeDir["freetype"] =	"Vendor/Lib/freetype/include"
IncludeDir["irrKlang"] =	"Vendor/Lib/irrKlang/include"
IncludeDir["json"] =		"Vendor/Lib/json/include"

include "Vendor/Lib/GLFW"
include "Vendor/Lib/GLAD"
include "Vendor/Lib/imgui"




project "RaggedSkeletonClient"
	location "Source"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("Temp/Binary/" .. outputdir .. "/%{prj.name}")
	objdir ("Temp/Intermediates/" .. outputdir .. "/%{prj.name}")

	pchheader "SkelPCH.h"
	pchsource "Source/PCH/SkelPCH.cpp"

	files
	{
		"Source/**.h",
		"Source/**.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"Source",
		"Source/SkelGame",
		"Source/PCH",
		"Source/Utility",
		"Source/Vendor",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.Assimp}",
		"%{IncludeDir.freetype}",
		"%{IncludeDir.irrKlang}"
	}

	links
	{
		"GLFW",
		"GLAD",
		"ImGui",
		"Vendor/Lib/Assimp/lib/assimp.lib",
		"Vendor/Lib/freetype/lib/freetype.lib",
		"Vendor/Lib/irrKlang/lib/Winx64-visualStudio/irrKlang.lib",
		"opengl32.lib",
		"Ws2_32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"SKEL_PLATFORM_WINDOWS",
			"SKEL_BUILD_DLL",
			"GLFW_INCLUDE_NONE",
			"CLIENT"
		}

	filter "configurations:Debug"
		defines 
		{
			"SKEL_DEBUG",
			"ENABLE_ASSERTS"
		}
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "SKEL_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "SKEL_DIST"
		runtime "Release"
		optimize "on"
	
	


project "RaggedSkeletonServer"
location "Source"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("Temp/Binary/" .. outputdir .. "/%{prj.name}")
	objdir ("Temp/Intermediates/" .. outputdir .. "/%{prj.name}")

	pchheader "SkelPCH.h"
	pchsource "Source/PCH/SkelPCH.cpp"

	files
	{
		"Source/**.h",
		"Source/**.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"Source",
		"Source/SkelGame",
		"Source/PCH",
		"Source/Utility",
		"Source/Vendor",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.Assimp}",
		"%{IncludeDir.freetype}",
		"%{IncludeDir.irrKlang}"
	}

	links
	{
		"GLFW",
		"GLAD",
		"ImGui",
		"Vendor/Lib/Assimp/lib/assimp.lib",
		"Vendor/Lib/freetype/lib/freetype.lib",
		"Vendor/Lib/irrKlang/lib/Winx64-visualStudio/irrKlang.lib",
		"opengl32.lib",
		"Ws2_32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"SKEL_PLATFORM_WINDOWS",
			"SKEL_BUILD_DLL",
			"GLFW_INCLUDE_NONE",
			"SERVER"
		}

	filter "configurations:Debug"
		defines 
		{
			"SKEL_DEBUG",
			"ENABLE_ASSERTS"
		}
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "SKEL_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "SKEL_DIST"
		runtime "Release"
		optimize "on"
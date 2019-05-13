workspace "NinthEngine"
	architecture "x64"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

includedir = {}
includedir["glew"] = "NinthEngine/lib/glew-2.1.0/include"
includedir["glm"] = "NinthEngine/lib/glm/glm"
includedir["spdlog"] = "NinthEngine/lib/spdlog/include"

include "NinthEngine/lib/glew-2.1.0"

project "NinthEngine"
	location "NinthEngine"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "NinthEngine/src/pch.cpp"
	
	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"%{prj.name}/src",
		"%{includedir.glew}",
		"%{includedir.glm}",
		"%{includedir.spdlog}"
	}
	
	links {
		"glew",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines {
			"NE_PLATFORM_WIN32",
			"NE_BUILD_DLL"
		}

		postbuildcommands {
			"{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .."/Test"
		}

	filter "configurations:Debug"
		defines "NE_DEBUG"
		symbols "On"
		
	filter "configurations:Release"
		defines "NE_RELEASE"
		optimize "On"
		
	filter "configurations:Dist"
		defines "NE_DIST"
		optimize "On"

project "Test"
	location "Test"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"NinthEngine/src",
		"%{includedir.glm}",
		"%{includedir.spdlog}"		
	}
	
	links {
		"NinthEngine"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines {
			"NE_PLATFORM_WIN32"
		}

	filter "configurations:Debug"
		defines "NE_DEBUG"
		symbols "On"
		
	filter "configurations:Release"
		defines "NE_RELEASE"
		optimize "On"
		
	filter "configurations:Dist"
		defines "NE_DIST"
		optimize "On"
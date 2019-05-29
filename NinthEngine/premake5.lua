project "NinthEngine"
	kind "SharedLib"
	language "C++"

	targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "include/NEPCH.h"
	pchsource "src/NEPCH.cpp"
	
	files {
		"include/**.h",
		"src/**.cpp"
	}

	includedirs {
		"include",
		"../%{includedir.glm}",
		"../%{includedir.spdlog}",
		"../%{includedir.stb}",
		"../%{includedir.tinyobjloader}",
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines {
			"NE_PLATFORM_WIN32",
			"NE_BUILD_DLL"
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
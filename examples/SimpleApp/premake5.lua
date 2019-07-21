project "SimpleApp"
	kind "ConsoleApp"
	language "C++"

	targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../../bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"src/**.cpp"
	}

	includedirs {
		"../../NinthEngine/include",
		"../../modules/OpenGL/include",
		"../../%{includedir.glew}",
		"../../%{includedir.glm}",
		"../../%{includedir.spdlog}",
		"../../%{includedir.stb}",
		"../../%{includedir.tinyobjloader}",
	}
	
	links {
		"NinthEngine",
		"OpenGL",
		"glew",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines {
			"NE_PLATFORM_WIN32"
		}

		postbuildcommands {
			"{COPY} ../../bin/" .. outputdir .. "/NinthEngine ../../bin/" .. outputdir .."/%{prj.name}",
			"{COPY} ../../bin/" .. outputdir .. "/OpenGL ../../bin/" .. outputdir .."/%{prj.name}",
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
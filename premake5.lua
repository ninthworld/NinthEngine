workspace "NinthEngine"
	architecture "x64"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

includedir = {}
includedir["glew"] = "dependencies/glew-2.1.0/include"
includedir["glm"] = "dependencies/glm"
includedir["spdlog"] = "dependencies/spdlog/include"
includedir["stb"] = "dependencies/stb"
includedir["tinyobjloader"] = "dependencies/tinyobjloader"

include "dependencies/glew-2.1.0"
include "NinthEngine"
include "modules/OpenGL"
include "examples/SimpleApp"

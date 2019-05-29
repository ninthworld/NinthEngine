project "GLEW"
    kind "StaticLib"
    language "C"
    
	targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../../bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
        "include/GL/eglew.h",
        "include/GL/glew.h",
        "include/GL/glxew.h",
        "src/glew.c",
        "src/glewinfo.c",
        "src/visualinfo.c"
    }
	
	includedirs {
		"include/"
	}
		
	defines
	{
		"GLEW_STATIC"
	}
    
	filter "system:windows"
        buildoptions { "-std=c11", "-lgdi32" }
        systemversion "10.0.16299.0"
        staticruntime "On"
        
		files
		{
			"include/GL/wglew.h"
		}
	
		defines 
		{ 
            "WIN32",
			"WIN32_LEAN_AND_MEAN"
		}
    filter { "system:windows", "configurations:Release" }
        buildoptions "/MT"
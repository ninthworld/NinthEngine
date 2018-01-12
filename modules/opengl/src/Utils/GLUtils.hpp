#pragma once

#define GLEW_STATIC
#include <GL\glew.h>

#ifdef _WIN32
#include <GL\wglew.h>
#endif

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32s.lib")

#include <plog\Log.h>

#ifdef _DEBUG
#define CHECK_ERROR(func) switch(glGetError()) { \
case GL_INVALID_ENUM: LOG_WARNING << func << ": " << "GL_INVALID_ENUM"; break; \
case GL_INVALID_VALUE: LOG_WARNING << func << ": " << "GL_INVALID_VALUE"; break; \
case GL_INVALID_OPERATION: LOG_WARNING << func << ": " << "GL_INVALID_OPERATION"; break; \
case GL_INVALID_FRAMEBUFFER_OPERATION: LOG_WARNING << func << ": " << "GL_INVALID_FRAMEBUFFER_OPERATION"; break; \
case GL_OUT_OF_MEMORY: LOG_WARNING << func << ": " << "GL_OUT_OF_MEMORY"; break; }
#else
#define CHECK_ERROR(func)
#endif

#include <NinthEngine\Render\Config\TextureConfig.hpp>

static const GLenum getGLenumType(const NinthEngine::FormatType format) {

	switch (format) {
	case NinthEngine::FORMAT_DEPTH_24_STENCIL_8: return GL_UNSIGNED_INT_24_8;
	default: return GL_UNSIGNED_BYTE;
	}
}

static const GLenum getGLenumFormat(const NinthEngine::FormatType format) {
	
	if (format & FORMAT_DEPTH) {
		if (format == NinthEngine::FORMAT_DEPTH_24_STENCIL_8 || 
			format == NinthEngine::FORMAT_DEPTH_32F_STENCIL_8) {
			return GL_DEPTH_STENCIL;
		}
		return GL_DEPTH_COMPONENT;
	}
	else if (format & FORMAT_R) {
		return GL_RED;
	}
	else if (format & FORMAT_RG) {
		return GL_RG;
	}
	else if (format & FORMAT_RGB) {
		return GL_RGB;
	}
	else {
		return GL_RGBA;
	}
}

static const GLenum getGLintIFormat(const NinthEngine::FormatType format) {

	return getGLenumFormat(format);
}


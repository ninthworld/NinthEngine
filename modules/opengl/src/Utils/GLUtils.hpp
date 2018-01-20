#pragma once

#define GLEW_STATIC
#include <GL\glew.h>

#ifdef _WIN32
#include <GL\wglew.h>
#endif

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32s.lib")

#include <NinthEngine\Utils\LogUtils.hpp>

#define CHECK_ERROR(func) switch(glGetError()) { \
case GL_INVALID_ENUM: LOG_WARNING << func << ": " << "GL_INVALID_ENUM"; break; \
case GL_INVALID_VALUE: LOG_WARNING << func << ": " << "GL_INVALID_VALUE"; break; \
case GL_INVALID_OPERATION: LOG_WARNING << func << ": " << "GL_INVALID_OPERATION"; break; \
case GL_INVALID_FRAMEBUFFER_OPERATION: LOG_WARNING << func << ": " << "GL_INVALID_FRAMEBUFFER_OPERATION"; break; \
case GL_OUT_OF_MEMORY: LOG_WARNING << func << ": " << "GL_OUT_OF_MEMORY"; break; }

#include <NinthEngine\Render\Config\TextureConfig.hpp>

static const GLenum getGLenumType(const NinthEngine::FormatType format) {

	if (format == NinthEngine::FORMAT_DEPTH_24_STENCIL_8) {
		return GL_UNSIGNED_INT_24_8;
	}
	else if (format & FORMAT_16) {
		return GL_UNSIGNED_SHORT;
	}
	else {
		return GL_UNSIGNED_BYTE;
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

	if (format & FORMAT_16) {
		if (format & FORMAT_R) {
			return GL_R16;
		}
		else if (format & FORMAT_RG) {
			return GL_RG16;
		}
		else if (format & FORMAT_RGB) {
			return GL_RGB16;
		}
		else {
			return GL_RGBA16;
		}
	}
	else {
		return getGLenumFormat(format);
	}
}

#include <NinthEngine\Render\Config\BlenderConfig.hpp>

static const GLenum getGLBlendEquation(const NinthEngine::BlendOpType func) {

	switch (func) {
	case NinthEngine::BLEND_OP_ADD: return GL_FUNC_ADD;
	case NinthEngine::BLEND_OP_SUBTRACT: return GL_FUNC_SUBTRACT;
	case NinthEngine::BLEND_OP_REV_SUBTRACT: return GL_FUNC_REVERSE_SUBTRACT;
	case NinthEngine::BLEND_OP_MIN: return GL_MIN;
	case NinthEngine::BLEND_OP_MAX: return GL_MAX;
	default: return GL_FUNC_ADD;
	}
}

static const GLenum getGLBlendType(const NinthEngine::BlendType type) {

	switch (type) {
	case NinthEngine::BLEND_ZERO: return GL_ZERO;
		case NinthEngine::BLEND_ONE: return GL_ONE;
		case NinthEngine::BLEND_SRC_COLOR: return GL_SRC_COLOR;
		case NinthEngine::BLEND_INV_SRC_COLOR: return GL_ONE_MINUS_SRC_COLOR;
		case NinthEngine::BLEND_SRC_ALPHA: return GL_SRC_ALPHA;
		case NinthEngine::BLEND_INV_SRC_ALPHA: return GL_ONE_MINUS_SRC_ALPHA;
		case NinthEngine::BLEND_DEST_ALPHA: return GL_DST_ALPHA;
		case NinthEngine::BLEND_INV_DEST_ALPHA: return GL_ONE_MINUS_DST_ALPHA;
		case NinthEngine::BLEND_DEST_COLOR: return GL_DST_COLOR;
		case NinthEngine::BLEND_INV_DEST_COLOR: return GL_ONE_MINUS_DST_COLOR;
		case NinthEngine::BLEND_SRC_ALPHA_SAT: return GL_SRC_ALPHA;	// Unique DirectX Blend Type?
		case NinthEngine::BLEND_BLEND_FACTOR: return GL_CONSTANT_COLOR;
		case NinthEngine::BLEND_INV_BLEND_FACTOR: return GL_ONE_MINUS_CONSTANT_COLOR;
		case NinthEngine::BLEND_SRC1_COLOR: return GL_SRC1_COLOR;
		case NinthEngine::BLEND_INV_SRC1_COLOR: return GL_ONE_MINUS_SRC1_COLOR;
		case NinthEngine::BLEND_SRC1_ALPHA: return GL_SRC1_ALPHA;
		case NinthEngine::BLEND_INV_SRC1_ALPHA: return GL_ONE_MINUS_SRC1_ALPHA;
		default: return GL_ZERO;
	}
}

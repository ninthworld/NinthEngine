#pragma once

#include "GLPCH.h"
#include <NinthEngine/Log.h>
#include <NinthEngine/Graphics/Texture.h>
#include <NinthEngine/Graphics/Sampler.h>

namespace NinthEngine {

#define NE_GL_CHECK_ERROR(func) { GLenum e = glGetError(); switch(e) { \
	case GL_INVALID_ENUM: Log::error("GL_INVALID_ENUM: " + e); break; \
	case GL_INVALID_VALUE: Log::error("GL_INVALID_VALUE: " + e); break; \
	case GL_INVALID_OPERATION: Log::error("GL_INVALID_OPERATION: " + e); break; \
	case GL_INVALID_FRAMEBUFFER_OPERATION: Log::error("GL_INVALID_FRAMEBUFFER_OPERATION: " + e); break; \
	case GL_OUT_OF_MEMORY: Log::error("GL_OUT_OF_MEMORY: " + e); break; } }

#define NE_GL_CHECK_FRAMEBUFFER_ERROR() { switch(glCheckFramebufferStatus(GL_FRAMEBUFFER)) { \
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: Log::error("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"); break; \
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: Log::error("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"); break; \
	case GL_FRAMEBUFFER_UNSUPPORTED: Log::error("GL_FRAMEBUFFER_UNSUPPORTED"); break; } }

inline static GLenum getGLTextureFormat(TextureComponentType components) {
	switch (components) {
	case TextureComponentType::Depth16:
	case TextureComponentType::Depth24:
	case TextureComponentType::Depth32:
	case TextureComponentType::Depth32F: return GL_DEPTH_COMPONENT;
	case TextureComponentType::Depth24Stencil8:
	case TextureComponentType::Depth32FStencil8:  return GL_DEPTH_STENCIL;
	case TextureComponentType::R: return GL_RED;
	case TextureComponentType::RG: return GL_RG;
	case TextureComponentType::RGB: return GL_RGB;
	case TextureComponentType::RGBA: default: return GL_RGBA;
	}
}

inline static GLenum getGLTextureIFormat(TextureFormatType format, TextureComponentType components) {
	switch (components) {
	case TextureComponentType::Depth16: return GL_DEPTH_COMPONENT16;
	case TextureComponentType::Depth24: return GL_DEPTH_COMPONENT24;
	case TextureComponentType::Depth32: return GL_DEPTH_COMPONENT32;
	case TextureComponentType::Depth32F: return GL_DEPTH_COMPONENT32F;
	case TextureComponentType::Depth24Stencil8: return GL_DEPTH24_STENCIL8;
	case TextureComponentType::Depth32FStencil8:  return GL_DEPTH32F_STENCIL8;
	case TextureComponentType::R: switch (format) {
		case TextureFormatType::Float16: return GL_R16F;
		case TextureFormatType::Float32: return GL_R32F;
		case TextureFormatType::SInt32: return GL_R32I;
		case TextureFormatType::UInt32: return GL_R32UI;
		case TextureFormatType::SInt16: return GL_R16I;
		case TextureFormatType::UInt16: return GL_R16UI;
		case TextureFormatType::SInt8: return GL_R8I;
		case TextureFormatType::UInt8: return GL_R8UI;
		case TextureFormatType::SNorm16: return GL_R16_SNORM;
		case TextureFormatType::UNorm16: return GL_R16;
		case TextureFormatType::SNorm8: return GL_R8_SNORM;
		case TextureFormatType::UNorm8: default: return GL_R8;
	} break;
	case TextureComponentType::RG: switch (format) {
		case TextureFormatType::Float16: return GL_RG16F;
		case TextureFormatType::Float32: return GL_RG32F;
		case TextureFormatType::SInt32: return GL_RG32I;
		case TextureFormatType::UInt32: return GL_RG32UI;
		case TextureFormatType::SInt16: return GL_RG16I;
		case TextureFormatType::UInt16: return GL_RG16UI;
		case TextureFormatType::SInt8: return GL_RG8I;
		case TextureFormatType::UInt8: return GL_RG8UI;
		case TextureFormatType::SNorm16: return GL_RG16_SNORM;
		case TextureFormatType::UNorm16: return GL_RG16;
		case TextureFormatType::SNorm8: return GL_RG8_SNORM;
		case TextureFormatType::UNorm8: default: return GL_RG8;
	} break;
	case TextureComponentType::RGB: switch (format) {
		case TextureFormatType::Float16: return GL_RGB16F;
		case TextureFormatType::Float32: return GL_RGB32F;
		case TextureFormatType::SInt32: return GL_RGB32I;
		case TextureFormatType::UInt32: return GL_RGB32UI;
		case TextureFormatType::SInt16: return GL_RGB16I;
		case TextureFormatType::UInt16: return GL_RGB16UI;
		case TextureFormatType::SInt8: return GL_RGB8I;
		case TextureFormatType::UInt8: return GL_RGB8UI;
		case TextureFormatType::SNorm16: return GL_RGB16_SNORM;
		case TextureFormatType::UNorm16: return GL_RGB16;
		case TextureFormatType::SNorm8: return GL_RGB8_SNORM;
		case TextureFormatType::UNorm8: default: return GL_RGB8;
	} break;
	case TextureComponentType::RGBA: default: switch (format) {
		case TextureFormatType::Float16: return GL_RGBA16F;
		case TextureFormatType::Float32: return GL_RGBA32F;
		case TextureFormatType::SInt32: return GL_RGBA32I;
		case TextureFormatType::UInt32: return GL_RGBA32UI;
		case TextureFormatType::SInt16: return GL_RGBA16I;
		case TextureFormatType::UInt16: return GL_RGBA16UI;
		case TextureFormatType::SInt8: return GL_RGBA8I;
		case TextureFormatType::UInt8: return GL_RGBA8UI;
		case TextureFormatType::SNorm16: return GL_RGBA16_SNORM;
		case TextureFormatType::UNorm16: return GL_RGBA16;
		case TextureFormatType::SNorm8: return GL_RGBA8_SNORM;
		case TextureFormatType::UNorm8: default: return GL_RGBA8;
	} break;
	}
}

inline static GLenum getGLTextureType(TextureFormatType format) {
	switch (format) {
	case TextureFormatType::Float16: return GL_HALF_FLOAT;
	case TextureFormatType::Float32: return GL_FLOAT;
	case TextureFormatType::SInt32: return GL_INT;
	case TextureFormatType::UInt32: return GL_UNSIGNED_INT;
	case TextureFormatType::SInt16:
	case TextureFormatType::SNorm16: return GL_SHORT;
	case TextureFormatType::UInt16:
	case TextureFormatType::UNorm16: return GL_UNSIGNED_SHORT;
	case TextureFormatType::SInt8:
	case TextureFormatType::SNorm8: return GL_BYTE;
	case TextureFormatType::UInt8:
	case TextureFormatType::UNorm8: default: return GL_UNSIGNED_BYTE;
	}
}

inline static GLint getGLSamplerFilterType(
	SamplerFilterType filter, 
	bool mipmapping = false, 
	SamplerFilterType mipmapFilter = SamplerFilterType::Linear) {
	if (mipmapping) {
		switch (mipmapFilter) {
		case SamplerFilterType::Linear:
			switch (filter) {
			case SamplerFilterType::Linear: return GL_LINEAR_MIPMAP_LINEAR;
			case SamplerFilterType::Nearest: return GL_NEAREST_MIPMAP_LINEAR;
			} break;
		case SamplerFilterType::Nearest:
			switch (filter) {
			case SamplerFilterType::Linear: return GL_LINEAR_MIPMAP_NEAREST;
			case SamplerFilterType::Nearest: return GL_NEAREST_MIPMAP_NEAREST;
			} break;
		}
	}
	else {
		switch (filter) {
		case SamplerFilterType::Linear: return GL_LINEAR;
		case SamplerFilterType::Nearest: return GL_NEAREST;
		}
	}
}

inline static GLint getGLSamplerEdgeType(SamplerEdgeType edge) {
	switch (edge) {
	case SamplerEdgeType::Wrap: return GL_REPEAT;
	case SamplerEdgeType::Mirror: return GL_MIRRORED_REPEAT;
	case SamplerEdgeType::Clamp: return GL_CLAMP_TO_EDGE;
	case SamplerEdgeType::Border: return GL_CLAMP_TO_BORDER;
	}
}

inline static GLenum getGLSamplerCompareType(SamplerCompareType compare) {
	switch (compare) {
	case SamplerCompareType::LessEqual: return GL_LEQUAL;
	case SamplerCompareType::GreaterEqual: return GL_GEQUAL;
	case SamplerCompareType::Less: return GL_LESS;
	case SamplerCompareType::Greater: return GL_GREATER;
	case SamplerCompareType::Equal: return GL_EQUAL;
	case SamplerCompareType::NotEqual: return GL_NOTEQUAL;
	case SamplerCompareType::Always: return GL_ALWAYS;
	case SamplerCompareType::Never: return GL_NEVER;
	}
}

}
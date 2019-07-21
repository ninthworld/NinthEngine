#pragma once

#include "GLPCH.h"
#include "GLCore.h"

namespace NinthEngine {

class NE_GL_API GLObject {
public:
	virtual const GLuint getNativeId() const = 0;
	virtual void destroy() = 0;
};

}
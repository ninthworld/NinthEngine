#pragma once

#include "GLCore.h"

namespace NinthEngine {

class NE_GL_API GLContext {
public:
	GLContext() {}
	virtual ~GLContext() {}

	virtual void makeCurrent() = 0;
	virtual void clearCurrent() = 0;
	virtual void releaseContext() = 0;
};

}
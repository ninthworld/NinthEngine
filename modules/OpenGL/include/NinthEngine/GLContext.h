#pragma once

#include "GLCore.h"

namespace NinthEngine {

class NE_GL_API GLContext {
public:
	virtual void makeCurrent() = 0;
	virtual void clearCurrent() = 0;
	virtual void releaseContext() = 0;
	virtual void swapBuffers() = 0;
};

}
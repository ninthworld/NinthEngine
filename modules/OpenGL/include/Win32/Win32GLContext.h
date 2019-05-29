#pragma once

#ifdef NE_PLATFORM_WIN32

#include "NEPCH.h"
#include "GLCore.h"
#include "GLContext.h"

namespace NinthEngine {

class NE_GL_API Win32GLContext : public GLContext {
public:
	Win32GLContext(HDC hdc, HGLRC glrc);
	~Win32GLContext();

	virtual void makeCurrent() override;
	virtual void clearCurrent() override;
	virtual void releaseContext() override;

private:
	HDC m_hdc;
	HGLRC m_glrc;
};

}

#endif
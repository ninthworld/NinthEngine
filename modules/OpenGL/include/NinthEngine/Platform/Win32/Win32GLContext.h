#pragma once

#ifdef NE_PLATFORM_WIN32

#include "NinthEngine/GLPCH.h"
#include "NinthEngine/GLCore.h"
#include "NinthEngine/GLContext.h"

namespace NinthEngine {

class NE_GL_API Win32GLContext : public GLContext {
public:
	Win32GLContext(HWND handle);
	~Win32GLContext();

	virtual void makeCurrent() override;
	virtual void clearCurrent() override;
	virtual void releaseContext() override;
	virtual void swapBuffers() override;

private:
	HWND m_handle;
	HDC m_hdc;
	HGLRC m_glrc;
};

}

#endif
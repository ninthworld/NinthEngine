#pragma once

namespace NinthEngine {

class GLContext {
public:
	GLContext() = default;
	GLContext(const GLContext&) = delete;
	GLContext& operator=(const GLContext&) = delete;

	virtual ~GLContext() = default;

	virtual void makeCurrent() = 0;
	virtual void clearCurrent() = 0;
	virtual void swapBuffers() = 0;

};

} // namespace NinthEngine
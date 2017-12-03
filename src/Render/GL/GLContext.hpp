#pragma once

namespace NinthEngine {

class GLContext {
public:
	virtual void makeCurrent() = 0;
	virtual void clearCurrent() = 0;
	virtual void swapBuffers() = 0;

};

} // namespace NinthEngine
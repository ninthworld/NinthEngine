#pragma once

#include <chrono>
#include "..\..\include\NinthEngine\Application\GameTimer.hpp"

namespace NinthEngine {

/* TEMPORARY */
/* TODO: Replace chrono with GLFW Timer */

class GLFWGameTimer : public GameTimer {
	typedef std::chrono::high_resolution_clock clock_t;

public:
	GLFWGameTimer() : start(clock_t::now()) {}
	~GLFWGameTimer() {};

	void reset() { start = clock_t::now(); };
	double elapsed() const { return std::chrono::duration_cast<std::chrono::nanoseconds>(clock_t::now() - start).count() / 1000000000.0; };

protected:
private:
	std::chrono::time_point<clock_t> start;

};

} // namespace NinthEngine
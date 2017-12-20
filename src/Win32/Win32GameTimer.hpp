#pragma once

#ifdef _WIN32

#include <chrono>
#include "..\..\include\NinthEngine\Application\GameTimer.hpp"

namespace NinthEngine {

class Win32GameTimer : public GameTimer {
	typedef std::chrono::high_resolution_clock clock_t;

public:
	Win32GameTimer() : m_start(clock_t::now()) {}
	~Win32GameTimer() {};

	void reset() override { m_start = clock_t::now(); };
	double elapsed() const override { return std::chrono::duration_cast<std::chrono::nanoseconds>(clock_t::now() - m_start).count() / 1000000000.0; };

protected:
private:
	std::chrono::time_point<clock_t> m_start;

};

} // namespace NinthEngine

#endif
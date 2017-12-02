#pragma once

#include <chrono>

namespace NinthEngine {

class GameTimer {
	typedef std::chrono::high_resolution_clock clock_t;

public:
	GameTimer() : start(clock_t::now()) {}
	virtual ~GameTimer() {}

	void reset();
	double elapsed() const;

protected:
private:
	std::chrono::time_point<clock_t> start;

};

inline void GameTimer::reset() {
	start = clock_t::now();
}

inline double GameTimer::elapsed() const {
	return std::chrono::duration_cast<std::chrono::nanoseconds>(clock_t::now() - start).count() / 1000000000.0;
}

} // namespace NinthEngine
#pragma once

#include <chrono>

namespace NinthEngine {

class GameTimer {
public:
	GameTimer() = default;
	GameTimer(const GameTimer&) = delete;
	GameTimer& operator=(const GameTimer&) = delete;

	virtual ~GameTimer() = default;

	virtual void reset() = 0;
	virtual double elapsed() const = 0;

};

} // namespace NinthEngine
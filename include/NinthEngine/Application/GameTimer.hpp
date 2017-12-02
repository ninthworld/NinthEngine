#pragma once

#include <chrono>

namespace NinthEngine {

class GameTimer {
public:
	virtual void reset() = 0;
	virtual double elapsed() const = 0;

};

} // namespace NinthEngine
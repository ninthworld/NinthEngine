#pragma once

#include "Builder\BlenderBuilder.hpp"

namespace NinthEngine {

class Blender {
public:
	Blender() = default;
	Blender(const Blender&) = delete;
	Blender& operator=(const Blender&) = delete;

	virtual ~Blender() = default;

};

} // namespace NinthEngine
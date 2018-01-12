#pragma once

namespace NinthEngine {

typedef unsigned ShaderTypeBit;

enum ShaderType : ShaderTypeBit {
	VERTEX_SHADER		= 0b1,
	HULL_SHADER			= 0b10,
	DOMAIN_SHADER		= 0b100,
	GEOMETRY_SHADER		= 0b1000,
	PIXEL_SHADER		= 0b10000,
	COMPUTE_SHADER		= 0b100000
};

} // namespace NinthEngine
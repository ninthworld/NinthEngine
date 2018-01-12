#pragma once

namespace NinthEngine {

enum FilterType {
	LINEAR,
	NEAREST
};

enum EdgeType {
	WRAP,
	MIRROR,
	CLAMP,
	BORDER
};

enum CompareType {
	NEVER,
	LESS,
	EQUAL,
	LESS_EQUAL,
	GREATER,
	NOT_EQUAL,
	GREATER_EQUAL,
	ALWAYS
};

} // namespace NinthEngine
#pragma once

#include "..\Resource\MathUtils.hpp"

namespace NinthEngine {

enum BufferTypeConfig {
	VERTEX_BT,
	INDEX_BT,
	CONSTANT_BT
};

class BufferConfig {
public:
	BufferConfig(const BufferTypeConfig);
	~BufferConfig();

	BufferConfig& setData(void* data, const size_t unitSize, const size_t totalSize);

	const BufferTypeConfig getType() { return type; };
	const size_t getTotalSize() { return totalSize; };
	const size_t getUnitSize() { return unitSize; };
	const void* getData() { return data; };

private:
	BufferTypeConfig type;
	size_t unitSize, totalSize;
	void* data;

};

} // namespace NinthEngine
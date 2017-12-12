#include "..\..\include\NinthEngine\Render\BufferConfig.hpp"

namespace NinthEngine {

BufferConfig::BufferConfig(const BufferTypeConfig type) 
	: type(type), unitSize(0), totalSize(0), data(nullptr) {
}

BufferConfig::~BufferConfig() {
}

BufferConfig& BufferConfig::setData(void* _data, const size_t _unitSize, const size_t _totalSize) {
	data = _data;
	unitSize = _unitSize;
	totalSize = _totalSize;
	return *this;
}

} // namespace NinthEngine
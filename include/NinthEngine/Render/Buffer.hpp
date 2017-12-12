#pragma once

namespace NinthEngine {

class Buffer {
public:
	virtual void bind() = 0;
	virtual void unbind() = 0;

};

} // namespace NinthEngine
#pragma once

#include <vector>
#include "InputLayoutConfig.hpp"

namespace NinthEngine {

enum BufferType {
	INDEX,
	VERTEX,
	CONSTANTS
};

class BufferConfig {
public:
	BufferConfig& asIndexBuffer() {
		m_type = INDEX;
		return *this;
	};

	BufferConfig& asVertexBuffer() {
		m_type = VERTEX;
		return *this;
	};

	BufferConfig& asConstantsBuffer() {
		m_type = CONSTANTS;
		return *this;
	};

	BufferConfig& setBinding(const unsigned binding) {
		m_binding = binding;
		return *this;
	}

	BufferConfig& setInputLayout(const InputLayoutConfig& config) {
		m_inputLayout = config;
		return *this;
	}

	BufferConfig& setData(void* data, const unsigned unitCount) {
		m_data = data;
		m_unitCount = unitCount;
		return *this;
	};

	BufferConfig& setData(const glm::mat4 data) {
		return setData((void*)glm::value_ptr(data), 1);
	}

protected:
	const unsigned getUnitSize() const {
		return m_inputLayout.m_unitSize;
	};

private:
	BufferType m_type;
	InputLayoutConfig m_inputLayout;
	unsigned m_binding = 0;
	void* m_data = nullptr;
	unsigned m_unitCount = 0;

	friend class GLGraphicsDevice;
	friend class GLIndexBuffer;
	friend class GLVertexBuffer;
	friend class GLConstantsBuffer;

	friend class D3DGraphicsDevice;
	friend class D3DIndexBuffer;
	friend class D3DVertexBuffer;
	friend class D3DConstantsBuffer;
};

} // namespace NinthEngine
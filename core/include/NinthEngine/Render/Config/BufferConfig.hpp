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
		m_config.m_type = INDEX;
		return *this;
	};

	BufferConfig& asVertexBuffer() {
		m_config.m_type = VERTEX;
		return *this;
	};

	BufferConfig& asConstantBuffer() {
		m_config.m_type = CONSTANTS;
		return *this;
	};

	BufferConfig& setBinding(const unsigned binding) {
		m_config.m_binding = binding;
		return *this;
	}

	BufferConfig& setInputLayout(const InputLayoutConfig& config) {
		m_config.m_inputLayout = config;
		return *this;
	}

	BufferConfig& setData(void* data, const unsigned unitCount) {
		m_config.m_data = data;
		m_config.m_unitCount = unitCount;
		return *this;
	};

	BufferConfig& setData(void* data) {
		return setData(data, 1);
	};

public:
	struct Config {
	BufferType m_type;
	InputLayoutConfig m_inputLayout;
	unsigned m_binding = 0;
	void* m_data = nullptr;
	unsigned m_unitCount = 0;
	} m_config;
};

} // namespace NinthEngine
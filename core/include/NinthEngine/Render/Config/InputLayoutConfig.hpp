#pragma once

#include <vector>

namespace NinthEngine {

enum InputLayoutType {
	INT1,
	SHORT1,
	FLOAT1,
	FLOAT2,
	FLOAT3,
	FLOAT4,
	MAT4
};

class InputLayoutConfig {
public:
	InputLayoutConfig& int1() {
		m_config.m_unitSize += sizeof(int);
		m_config.m_stack.push_back(INT1);
		return *this;
	}

	InputLayoutConfig& short1() {
		m_config.m_unitSize += sizeof(short);
		m_config.m_stack.push_back(SHORT1);
		return *this;
	}

	InputLayoutConfig& float1() {
		m_config.m_unitSize += sizeof(float);
		m_config.m_stack.push_back(FLOAT1);
		return *this;
	};

	InputLayoutConfig& float2() {
		m_config.m_unitSize += 2 * sizeof(float);
		m_config.m_stack.push_back(FLOAT2);
		return *this;
	};

	InputLayoutConfig& float3() {
		m_config.m_unitSize += 3 * sizeof(float);
		m_config.m_stack.push_back(FLOAT3);
		return *this;
	};

	InputLayoutConfig& float4() {
		m_config.m_unitSize += 4 * sizeof(float);
		m_config.m_stack.push_back(FLOAT4);
		return *this;
	};

	InputLayoutConfig& mat4() {
		m_config.m_unitSize += 16 * sizeof(float);
		m_config.m_stack.push_back(MAT4);
		return *this;
	};

public:
	struct Config {
	std::vector<InputLayoutType> m_stack;
	unsigned m_unitSize = 0;
	} m_config;
	
};

} // namespace NinthEngine
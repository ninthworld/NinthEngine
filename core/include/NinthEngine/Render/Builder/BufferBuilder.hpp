#pragma once

#include <plog\Log.h>
#include <vector>
#include <functional>
#include <memory>
#include "..\Config\LayoutConfig.hpp"

namespace NinthEngine {

class Buffer;

class BufferBuilder {
public:
	BufferBuilder(const std::function<
		std::unique_ptr<Buffer>(
		const LayoutConfig, 
		const unsigned, void*)>& build)
		: m_build(build)
		, m_layout(LayoutConfig())
		, m_unitCount(0)
		, m_data(nullptr) {};
	~BufferBuilder() {};

	std::unique_ptr<Buffer> build() {
		return std::move(m_build(m_layout, m_unitCount, m_data));
	};

	BufferBuilder& withLayout(const LayoutConfig layout) {
		m_layout = layout;
		return *this;
	};

	BufferBuilder& withData(const unsigned unitCount, void* data) {
		m_unitCount = unitCount;
		m_data = data;
		return *this;
	};
	
	BufferBuilder& withData(void* data) {
		return withData(1, data);
	};

private:
	std::function<
		std::unique_ptr<Buffer>(
		const LayoutConfig, 
		const unsigned, void*)> m_build;

	LayoutConfig m_layout;
	unsigned m_unitCount;
	void* m_data;
};

} // namespace NinthEngine
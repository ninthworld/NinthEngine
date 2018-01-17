#pragma once

#include <vector>
#include <functional>
#include <memory>
#include "..\Config\LayoutConfig.hpp"
#include "..\VertexBuffer.hpp"

namespace NinthEngine {

// TODO: Use actual OOP on this mess somehow...

class BufferBuilder {
public:
	BufferBuilder()
		: m_layout(LayoutConfig())
		, m_unitCount(0)
		, m_data(nullptr) {};
	~BufferBuilder() {};

protected:
	LayoutConfig m_layout;
	unsigned m_unitCount;
	void* m_data;

};

class VertexBufferBuilder : public BufferBuilder {
public:
	VertexBufferBuilder(const std::function<
		std::unique_ptr<VertexBuffer>(
			const LayoutConfig,
			const unsigned, void*)>& build)
		: m_build(build)
		, BufferBuilder() {};
	~VertexBufferBuilder() {};

	std::unique_ptr<VertexBuffer> build() {
		return std::move(m_build(m_layout, m_unitCount, m_data));		
	};

	VertexBufferBuilder& withLayout(const LayoutConfig layout) {
		m_layout = layout;
		return *this;
	};

	VertexBufferBuilder& withData(const unsigned unitCount, void* data) {
		m_unitCount = unitCount;
		m_data = data;
		return *this;
	};

	VertexBufferBuilder& withData(void* data) {
		return withData(1, data);
	};

private:
	std::function<
		std::unique_ptr<VertexBuffer>(
			const LayoutConfig,
			const unsigned, void*)> m_build;

};

class IndexBufferBuilder : public BufferBuilder {
public:
	IndexBufferBuilder(const std::function<
		std::unique_ptr<IndexBuffer>(
			const LayoutConfig,
			const unsigned, void*)>& build)
		: m_build(build)
		, BufferBuilder() {};
	~IndexBufferBuilder() {};

	std::unique_ptr<IndexBuffer> build() {
		return std::move(m_build(m_layout, m_unitCount, m_data));
	};

	IndexBufferBuilder& withLayout(const LayoutConfig layout) {
		m_layout = layout;
		return *this;
	};

	IndexBufferBuilder& withData(const unsigned unitCount, void* data) {
		m_unitCount = unitCount;
		m_data = data;
		return *this;
	};

	IndexBufferBuilder& withData(void* data) {
		return withData(1, data);
	};

private:
	std::function<
		std::unique_ptr<IndexBuffer>(
			const LayoutConfig,
			const unsigned, void*)> m_build;

};

class ConstantBufferBuilder : public BufferBuilder {
public:
	ConstantBufferBuilder(const std::function<
		std::unique_ptr<ConstantBuffer>(
			const LayoutConfig,
			const unsigned, void*)>& build)
		: m_build(build)
		, BufferBuilder() {};
	~ConstantBufferBuilder() {};

	std::unique_ptr<ConstantBuffer> build() {
		return std::move(m_build(m_layout, m_unitCount, m_data));
	};

	ConstantBufferBuilder& withLayout(const LayoutConfig layout) {
		m_layout = layout;
		return *this;
	};

	ConstantBufferBuilder& withData(const unsigned unitCount, void* data) {
		m_unitCount = unitCount;
		m_data = data;
		return *this;
	};

	ConstantBufferBuilder& withData(void* data) {
		return withData(1, data);
	};

private:
	std::function<
		std::unique_ptr<ConstantBuffer>(
			const LayoutConfig,
			const unsigned, void*)> m_build;

};

} // namespace NinthEngine
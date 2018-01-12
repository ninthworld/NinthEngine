#pragma once

#include <plog\Log.h>
#include <vector>
#include <functional>
#include <memory>
#include "..\Config\ShaderConfig.hpp"
#include "..\Config\LayoutConfig.hpp"

namespace NinthEngine {

struct ShaderStruct {
	ShaderType type;
	std::string file;
	std::string entry;
};

class Shader;

class ShaderBuilder {
public:
	ShaderBuilder(const std::function<
		std::unique_ptr<Shader>(
		const LayoutConfig, 
		std::vector<ShaderStruct>, 
		std::vector<ShaderStruct>)>& build)
		: m_build(build)
		, m_layout(LayoutConfig()) {};
	~ShaderBuilder() {};

	std::unique_ptr<Shader> build() {
		return std::move(m_build(m_layout, m_hlslShaders, m_glslShaders));
	};

	ShaderBuilder& withLayout(const LayoutConfig layout) {
		m_layout = layout;
		return *this;
	};

	template<ShaderType T>
	ShaderBuilder& withHLSL(const std::string file, const std::string entry) {
		m_hlslShaders.push_back({ T, file, entry });
		return *this;
	};

	template<ShaderType T>
	ShaderBuilder& withGLSL(const std::string file) {
		m_glslShaders.push_back({ T, file });
		return *this;
	};


private:
	std::function<
		std::unique_ptr<Shader>(
		const LayoutConfig, 
		std::vector<ShaderStruct>,
		std::vector<ShaderStruct>)> m_build;

	LayoutConfig m_layout;
	std::vector<ShaderStruct> m_hlslShaders;
	std::vector<ShaderStruct> m_glslShaders;

};

} // namespace NinthEngine
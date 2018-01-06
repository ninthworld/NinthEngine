#pragma once

#include <memory>
#include <map>
#include <NinthEngine\Render\Shader.hpp>
#include "..\Utils\GLUtils.hpp"

namespace NinthEngine {
namespace GL {

class GLShader : public Shader {
public:
	GLShader();
	~GLShader();

	void createVertexShader(const ShaderConfig& config);
	void createHullShader(const ShaderConfig& config);
	void createDomainShader(const ShaderConfig& config);
	void createGeometryShader(const ShaderConfig& config);
	void createPixelShader(const ShaderConfig& config);
	void createProgram();

	void bindConstant(const std::string name, const std::shared_ptr<ConstantBuffer>& buffer) override;
	void bindTexture(const std::string name, const std::shared_ptr<Texture>& texture) override;

	void bind() override;
	void unbind() override;
	
private:
	GLuint m_programId;
	GLuint m_vertexShader;
	GLuint m_hullShader;
	GLuint m_domainShader;
	GLuint m_geometryShader;
	GLuint m_pixelShader;

};

} // namespace GL
} // namespace NinthEngine

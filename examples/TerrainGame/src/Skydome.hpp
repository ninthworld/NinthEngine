#pragma once

#include <memory>
#include <NinthEngine\Camera\GameCamera.hpp>
#include <NinthEngine\Render\GraphicsDevice.hpp>
#include <NinthEngine\Render\GraphicsContext.hpp>
#include <NinthEngine\Render\IndexBuffer.hpp>
#include <NinthEngine\Render\VertexBuffer.hpp>
#include <NinthEngine\Render\VertexArray.hpp>
#include <NinthEngine\Render\Texture.hpp>
#include <NinthEngine\Render\Shader.hpp>

using namespace NinthEngine;

class Skydome {
	struct SkydomeData {
		glm::vec4 skyColor;
		glm::vec4 scale;
	};

public:
	Skydome(
		const std::shared_ptr<GraphicsDevice>& device, 
		const std::shared_ptr<GraphicsContext>& context,
		const std::shared_ptr<GameCamera>& camera,
		const std::shared_ptr<ConstantBuffer>& constantCamera);
	~Skydome();

	void render();

private:
	std::shared_ptr<GraphicsContext> m_context;
	std::shared_ptr<GameCamera> m_camera;

	// Shader
	std::unique_ptr<Shader> m_shader;

	// Constants
	std::shared_ptr<ConstantBuffer> m_constantCamera;
	std::shared_ptr<ConstantBuffer> m_constantSkydome;

	// Textures
	std::shared_ptr<Texture> m_texture;

	// Index Buffers
	std::shared_ptr<IndexBuffer> m_indexBuffer;

	// Vertex Buffers
	std::shared_ptr<VertexBuffer> m_vertexBuffer;

	// Vertex Array
	std::shared_ptr<VertexArray> m_vertexArray;

	// Skydome Data
	SkydomeData m_skydomeData;
};
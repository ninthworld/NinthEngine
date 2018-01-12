#pragma once

#include <memory>
#include <NinthEngine\Application\Game.hpp>

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
		const std::shared_ptr<Buffer>& constantCamera);
	~Skydome();

	void render();

private:
	std::shared_ptr<GraphicsContext> m_context;
	std::shared_ptr<GameCamera> m_camera;

	// Shader
	std::shared_ptr<Shader> m_shader;

	// Constants
	std::shared_ptr<Buffer> m_constantCamera;
	std::shared_ptr<Buffer> m_constantSkydome;

	// Samplers
	std::shared_ptr<Sampler> m_sampler;

	// Textures
	std::shared_ptr<Texture> m_texture;

	// Index Buffers
	std::shared_ptr<Buffer> m_indexBuffer;

	// Vertex Buffers
	std::shared_ptr<Buffer> m_vertexBuffer;

	// Vertex Array
	std::shared_ptr<VertexArray> m_vertexArray;

	// Skydome Data
	SkydomeData m_skydomeData;
};
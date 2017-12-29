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
public:
	Skydome(
		const std::shared_ptr<GraphicsDevice>& device, 
		const std::shared_ptr<GraphicsContext>& context,
		const std::shared_ptr<GameCamera>& camera);
	~Skydome();

	void render();

private:
	std::shared_ptr<GraphicsContext> m_context;
	std::shared_ptr<GameCamera> m_camera;

	std::unique_ptr<Shader> m_shader;
	std::shared_ptr<ConstantsBuffer> m_constantsMVP;
	std::shared_ptr<Texture> m_texture;
	std::shared_ptr<IndexBuffer> m_indexBuffer;
	std::shared_ptr<VertexBuffer> m_vertexBuffer;
	std::shared_ptr<VertexArray> m_vertexArray;

};
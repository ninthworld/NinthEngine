#pragma once

#include <memory>
#include <NinthEngine\Application\Game.hpp>

namespace NinthEngine {

class GraphicsDevice;
class GraphicsContext;
class Shader;
class Texture;
class VertexBuffer;
class IndexBuffer;
class ConstantsBuffer;
class VertexArray;
class GameWindow;
class GameCamera;

} // namespace NinthEngine

using namespace NinthEngine;

class TerrainNode;

enum NeighborEdge : unsigned {
	NONE	= 0x0,
	NORTH	= 0b1,
	SOUTH	= 0b10,
	EAST	= 0b100,
	WEST	= 0b1000
};

class Terrain {
public:
	Terrain(
		const std::shared_ptr<GraphicsDevice>& device,
		const std::shared_ptr<GraphicsContext>& context,
		const std::shared_ptr<GameCamera>& camera);
	~Terrain();

	void update();

	void render();

private:
	std::shared_ptr<GraphicsContext> m_context;
	std::shared_ptr<GameCamera> m_camera;
	glm::vec3 m_camLastPos;

	std::unique_ptr<Shader> m_shader;
	std::shared_ptr<ConstantsBuffer> m_constantsViewProj;
	std::shared_ptr<ConstantsBuffer> m_constantsModel;
	std::shared_ptr<Texture> m_textureHeight;
	std::shared_ptr<Texture> m_textureNormal;
	std::shared_ptr<Texture> m_textureRockDiffuse;
	std::shared_ptr<Texture> m_textureRockNormal;

	std::shared_ptr<VertexBuffer> m_vertexBuffer;
	std::shared_ptr<VertexArray> m_vertexArray;

	std::vector<std::shared_ptr<IndexBuffer>> m_indexBuffers;
	std::shared_ptr<TerrainNode> m_quadTree;

};
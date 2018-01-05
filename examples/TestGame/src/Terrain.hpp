#pragma once

#include <memory>
#include <NinthEngine\Application\Game.hpp>

const static int rootPatches = 8;
const static float scaleXZ = 6000.0f;
const static float scaleY = 400.0f;
const static float texDetail = 1000.0f;
const static int tessFactor = 600;
const static float tessSlope = 1.8f;
const static float tessShift = -0.3f;
const static int detailRange = 400;
const static float sightRangeFactor = 2.0f;
const static float lodRange[8]{
	1750,
	874,
	386,
	192,
	100,
	50,
	0,
	0
};

namespace NinthEngine {

class GraphicsDevice;
class GraphicsContext;
class Shader;
class Texture;
class VertexBuffer;
class IndexBuffer;
class ConstantBuffer;
class VertexArray;
class GameWindow;
class GameCamera;

} // namespace NinthEngine

using namespace NinthEngine;

struct TerrainData {
	glm::mat4 worldMatrix;
	glm::vec4 scale;
};

struct NodeData {
	glm::mat4 localMatrix;
	glm::vec2 location;
	glm::vec2 index;
	float size;
	int lod;
	glm::vec2 _padding;
};

class TerrainNode;

class Terrain {
public:
	Terrain(
		const std::shared_ptr<GraphicsDevice>& device,
		const std::shared_ptr<GraphicsContext>& context,
		const std::shared_ptr<GameCamera>& camera,
		const std::shared_ptr<ConstantBuffer>& constantCamera);
	~Terrain();

	void update();

	void render();

private:
	std::shared_ptr<GraphicsContext> m_context;
	std::shared_ptr<GameCamera> m_camera;
	glm::vec3 m_camLastPos;

	// Shader
	std::unique_ptr<Shader> m_shader;
	
	// Constant Buffers
	std::shared_ptr<ConstantBuffer> m_constantCamera;
	std::shared_ptr<ConstantBuffer> m_constantTerrain;
	std::shared_ptr<ConstantBuffer> m_constantNode;

	// Textures
	std::shared_ptr<Texture> m_heightmap;
	std::shared_ptr<Texture> m_normalmap;

	// Index Buffers
	std::shared_ptr<IndexBuffer> m_indexBuffer;

	// Vertex Buffers
	std::shared_ptr<VertexBuffer> m_vertexBuffer;

	// Vertex Array
	std::shared_ptr<VertexArray> m_vertexArray;

	// Terrain Root Nodes
	std::vector<std::unique_ptr<TerrainNode>> m_rootNodes;

};
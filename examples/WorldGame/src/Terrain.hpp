#pragma once

#include <memory>
#include <vector>
#include <NinthEngine\Render\GraphicsContext.hpp>
#include <NinthEngine\Render\GraphicsDevice.hpp>
#include <NinthEngine\Camera\GameCamera.hpp>
#include <NinthEngine\Input\Keyboard.hpp>

using namespace NinthEngine;

struct TerrainConfig {
	glm::vec3 scale;
	int nodeVertexCount;
	int rootCount;
	float lodRanges[16];
};

struct TerrainStruct {
	glm::vec4 terrainScale;
};

struct NodeStruct {
	glm::vec2 nodeLocalPos;
	float nodeScale;
	int nodeLod;
};

typedef unsigned NodeNeighborBit;
enum NodeNeighborType : NodeNeighborBit {
	NORTH	= 0b1,
	SOUTH	= 0b10,
	WEST	= 0b100,
	EAST	= 0b1000
};

class TerrainNode;

class Terrain : public std::enable_shared_from_this<Terrain> {
public:
	Terrain(
		const std::shared_ptr<GraphicsDevice>& device,
		const std::shared_ptr<GraphicsContext>& context,
		const std::shared_ptr<GameCamera>& camera,
		const std::shared_ptr<ConstantBuffer>& constantCamera,
		const TerrainConfig config);
	~Terrain();

	void init();
	void update();
	void render();

	void onKeyboard(const Key key, const KeyState state);

	const int getLodAt(const glm::vec2 localPos);

private:
	std::shared_ptr<GraphicsContext> m_context;
	std::shared_ptr<GameCamera> m_camera;
	TerrainConfig m_config;
	std::vector<std::shared_ptr<TerrainNode>> m_children;

	bool m_wireframe;
	std::shared_ptr<Rasterizer> m_rasterizerDefault;
	std::shared_ptr<Rasterizer> m_rasterizerWireframe;

	std::shared_ptr<IndexBuffer> m_indexBuffers[16];
	std::shared_ptr<VertexArray> m_vertexArray;
	std::shared_ptr<Shader> m_shader;

	std::shared_ptr<ConstantBuffer> m_constantCamera;
	std::shared_ptr<ConstantBuffer> m_constantTerrain;
	std::shared_ptr<ConstantBuffer> m_constantNode;

	std::shared_ptr<Sampler> m_sampler;
	std::shared_ptr<Texture> m_heightmap;
	std::shared_ptr<Texture> m_normalmap;

	friend class TerrainNode;
};
#pragma once

#include <memory>
#include <NinthEngine\Application\Game.hpp>

const static int patchSize = 4;
const static int rootPatches = 8;
const static float scaleXZ = 6000.0f;
const static float scaleY = 400.0f;
const static int detailRangeFar = 400;
const static int detailRangeNear = 350;
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

using namespace NinthEngine;

struct TerrainData {
	glm::mat4 worldMatrix;
	float scaleXZ;
	float scaleY;
	float detailRangeNear;
	float detailRangeFar;
	float lodRange[8];
};

struct NodeData {
	glm::mat4 localMatrix;
	glm::vec4 neighbors;
	glm::vec2 location;
	glm::vec2 index;
	float size;
	int lod;
	glm::vec2 _padding;
};

struct Material {
	std::shared_ptr<Texture> diffuse;
	std::shared_ptr<Texture> displacement;
	std::shared_ptr<Texture> normal;
	std::shared_ptr<Texture> mapAlpha;
};

class TerrainNode;

class Terrain : public std::enable_shared_from_this<Terrain> {
public:
	Terrain(
		const std::shared_ptr<GraphicsDevice>& device,
		const std::shared_ptr<GraphicsContext>& context,
		const std::shared_ptr<GameCamera>& camera,
		const std::shared_ptr<ConstantBuffer>& constantCamera);
	~Terrain();
		
	void init();
	void update();
	void render();

	const int getLodAt(const glm::vec2 pos);
	const float getHeightAt(const glm::vec2 pos);
	const float getMaxHeightAt(const glm::vec2 pos0, const glm::vec2 pos1);

private:
	std::shared_ptr<GraphicsContext> m_context;
	std::shared_ptr<GameCamera> m_camera;
	glm::vec3 m_camLastPos;

	// Shader
	std::shared_ptr<Shader> m_shader;
	
	// Constant Buffers
	std::shared_ptr<ConstantBuffer> m_constantCamera;
	std::shared_ptr<ConstantBuffer> m_constantTerrain;
	std::shared_ptr<ConstantBuffer> m_constantNode;

	// Samplers
	std::shared_ptr<Sampler> m_sampler;

	// Textures
	std::shared_ptr<Texture> m_heightmap;
	std::shared_ptr<Texture> m_normalmap;
	
	// Materials
	std::vector<Material> m_materials;
	
	// Vertex Array
	std::shared_ptr<VertexArray> m_vertexArray;

	// Terrain Root Nodes
	std::vector<std::unique_ptr<TerrainNode>> m_rootNodes;

	// Terrain Height Data
	int m_heightWidth, m_heightHeight;
	float* m_heightData;
};
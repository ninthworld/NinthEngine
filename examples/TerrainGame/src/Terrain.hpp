#pragma once

#include <memory>
#include <NinthEngine\Application\Game.hpp>

const static int patchSize = 16;
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
	
	// Materials
	std::shared_ptr<Texture> m_material0Dif;
	std::shared_ptr<Texture> m_material0Disp;
	std::shared_ptr<Texture> m_material0Norm;
	std::shared_ptr<Texture> m_material0Alpha;

	std::shared_ptr<Texture> m_material1Dif;
	std::shared_ptr<Texture> m_material1Disp;
	std::shared_ptr<Texture> m_material1Norm;
	std::shared_ptr<Texture> m_material1Alpha;

	std::shared_ptr<Texture> m_material2Dif;
	std::shared_ptr<Texture> m_material2Disp;
	std::shared_ptr<Texture> m_material2Norm;
	std::shared_ptr<Texture> m_material2Alpha;

	std::shared_ptr<Texture> m_material3Dif;
	std::shared_ptr<Texture> m_material3Disp;
	std::shared_ptr<Texture> m_material3Norm;
	std::shared_ptr<Texture> m_material3Alpha;
	
	// Vertex Buffers
	std::shared_ptr<VertexBuffer> m_vertexBuffer;

	// Vertex Array
	std::shared_ptr<VertexArray> m_vertexArray;

	// Terrain Root Nodes
	std::vector<std::unique_ptr<TerrainNode>> m_rootNodes;

};
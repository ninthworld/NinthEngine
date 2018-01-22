#pragma once

#include <memory>
#include <vector>
#include <NinthEngine\Render\GraphicsContext.hpp>
#include <NinthEngine\Render\GraphicsDevice.hpp>
#include <NinthEngine\Camera\GameCamera.hpp>
#include <NinthEngine\Input\Keyboard.hpp>

using namespace NinthEngine;

const static LayoutConfig terrainLayout =
	LayoutConfig().float2(POSITION);

const static LayoutConfig terrainStructLayout =
	LayoutConfig().float4();
struct TerrainStruct {
	glm::vec4 terrainScale;
};
/*
* GLSL
	layout(std140) uniform Terrain {
		vec4 terrainScale;
	};

* HLSL
	cbuffer Terrain : register(b?) {
		float4 terrainScale;
	};
*/

const static LayoutConfig nodeStructLayout =
	LayoutConfig().float2().float1().int1();
struct NodeStruct {
	glm::vec2 nodeLocalPos;
	float nodeScale;
	int nodeLod;
};
/*
* GLSL
	layout(std140) uniform Node {
		vec2 nodeLocalPos;
		float nodeScale;
		int nodeLod;
	};

* HLSL
	cbuffer Node : register(b?) {
		float2 nodeLocalPos;
		float nodeScale;
		int nodeLod;
	};
*/

struct Material {
	std::shared_ptr<Texture> color;
	std::shared_ptr<Texture> normal;
};

struct TerrainConfig {
	glm::vec3 scale;
	int nodeVertexCount;
	int rootCount;
	float lodRanges[16];
};

typedef unsigned NodeNeighborBit;
enum NodeNeighborType : NodeNeighborBit {
	NORTH	= 0b1,
	SOUTH	= 0b10,
	WEST	= 0b100,
	EAST	= 0b1000
};

class Terrain;
class TerrainNode;

class Terrain : public std::enable_shared_from_this<Terrain> {
public:
	Terrain(
		const std::shared_ptr<GraphicsDevice>& device,
		const std::shared_ptr<GraphicsContext>& context,
		const std::shared_ptr<GameCamera>& camera,
		const std::shared_ptr<ConstantBuffer>& constantCamera,
		const std::shared_ptr<ConstantBuffer>& constantShadowMap,
		const TerrainConfig config);
	~Terrain();

	void init();
	void update();
	void preRender();
	void render();
	void renderShadows();

	void onKeyboard(const Key key, const KeyState state);

	const int getLodAt(const glm::vec2 localPos);

private:
	// Samplers
	std::shared_ptr<Sampler> m_sampler;
	void initSamplers(const std::shared_ptr<GraphicsDevice>& device);

	// Geometry
	std::shared_ptr<IndexBuffer> m_indexBuffers[16];
	std::shared_ptr<VertexArray> m_vertexArray;
	void initGeometry(const std::shared_ptr<GraphicsDevice>& device);

	// Constants
	std::shared_ptr<ConstantBuffer> m_constantTerrain;
	std::shared_ptr<ConstantBuffer> m_constantNode;
	void initConstants(const std::shared_ptr<GraphicsDevice>& device);

	// Textures
	std::shared_ptr<Texture> m_heightmap;
	std::shared_ptr<Texture> m_normalmap;
	std::shared_ptr<Texture> m_splatmap;
	std::vector<Material> m_materials;
	void initTextures(const std::shared_ptr<GraphicsDevice>& device);

	// Shaders
	std::shared_ptr<Shader> m_shader;
	std::shared_ptr<Shader> m_shaderShadow;
	void initShaders(
		const std::shared_ptr<GraphicsDevice>& device,
		const std::shared_ptr<ConstantBuffer>& constantCamera,
		const std::shared_ptr<ConstantBuffer>& constantShadowMap);

private:
	std::shared_ptr<GraphicsContext> m_context;
	std::shared_ptr<GameCamera> m_camera;

	TerrainConfig m_config;
	std::vector<std::shared_ptr<TerrainNode>> m_children;
	
	friend class TerrainNode;
};
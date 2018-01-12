#include <NinthEngine\Camera\GameCamera.hpp>
#include <NinthEngine\Render\GraphicsDevice.hpp>
#include <NinthEngine\Render\GraphicsContext.hpp>
#include <NinthEngine\Render\Shader.hpp>
#include <NinthEngine\Render\Buffer.hpp>
#include "TerrainNode.hpp"
#include "Terrain.hpp"

Terrain::Terrain(
	const std::shared_ptr<GraphicsDevice>& device,
	const std::shared_ptr<GraphicsContext>& context,
	const std::shared_ptr<GameCamera>& camera,
	const std::shared_ptr<Buffer>& constantCamera)
	: m_context(context)
	, m_camera(camera)
	, m_constantCamera(constantCamera) {
	
	auto inputLayout = LayoutConfig().float2(POSITION);

	// Generate Model Data
	std::vector<glm::vec2> vertices;
	const unsigned divs = sqrt(patchSize) - 1;
	const float fDivs = divs;

	for (unsigned i = 0; i <= divs; ++i) {
		for (unsigned j = 0; j <= divs; ++j) {
			vertices.push_back(glm::vec2(j / fDivs, i / fDivs));
		}
	}
	
	// Initialize Vertex Buffer
	m_vertexBuffer = device->createVertexBuffer()
		.withLayout(inputLayout)
		.withData(vertices.size(), vertices.data())
		.build();

	// Initialize Vertex Array
	m_vertexArray = device->createVertexArray();
	m_vertexArray->addVertexBuffer(m_vertexBuffer);

	// Initialize Shader
	m_shader = device->createShader()
		.withLayout(inputLayout)
		.withGLSL<VERTEX_SHADER>("res/terrain/shader/terrain.vs.glsl")
		.withGLSL<HULL_SHADER>("res/terrain/shader/terrain.hs.glsl")
		.withGLSL<DOMAIN_SHADER>("res/terrain/shader/terrain.ds.glsl")
		.withGLSL<GEOMETRY_SHADER>("res/terrain/shader/terrain.gs.glsl")
		.withGLSL<PIXEL_SHADER>("res/terrain/shader/terrain.ps.glsl")
		.withHLSL<VERTEX_SHADER>("res/terrain/shader/terrain.vs.hlsl", "main")
		.withHLSL<HULL_SHADER>("res/terrain/shader/terrain.hs.hlsl", "main")
		.withHLSL<DOMAIN_SHADER>("res/terrain/shader/terrain.ds.hlsl", "main")
		.withHLSL<GEOMETRY_SHADER>("res/terrain/shader/terrain.gs.hlsl", "main")
		.withHLSL<PIXEL_SHADER>("res/terrain/shader/terrain.ps.hlsl", "main")
		.build();
	m_shader->bindConstant("Camera", m_constantCamera);

	// Initialize Constant Buffers
	TerrainData terrainData{ glm::mat4(1), scaleXZ, scaleY, detailRangeNear, detailRangeFar };
	terrainData.worldMatrix = glm::translate(terrainData.worldMatrix, glm::vec3(-terrainData.scaleXZ / 2.0f, 0.0f, -terrainData.scaleXZ / 2.0f));
	terrainData.worldMatrix = glm::scale(terrainData.worldMatrix, glm::vec3(terrainData.scaleXZ, terrainData.scaleY, terrainData.scaleXZ));
	for (unsigned i = 0; i < 8; ++i) terrainData.lodRange[i] = lodRange[i];

	m_constantTerrain = device->createConstantBuffer()
		.withLayout(LayoutConfig().float4x4().float4().float4().float4())
		.withData(&terrainData)
		.build();
	m_constantTerrain->setBinding(1);
	m_shader->bindConstant("Terrain", m_constantTerrain);

	NodeData nodeData{};
	m_constantNode = device->createConstantBuffer()
		.withLayout(LayoutConfig().float4x4().float2().float2().float1().int1().float4().float2())
		.withData(&nodeData)
		.build();
	m_constantNode->setBinding(2);
	m_shader->bindConstant("Node", m_constantNode);

	// Initialize Samplers
	m_sampler = device->createSampler()
		.withFilter(LINEAR)
		.withEdge(WRAP)
		.withMipmapping(LINEAR, 0, 8)
		.build();
	m_sampler->setBinding(0);

	// Load Textures
	unsigned t = 0;
	m_heightmap = device->createTexture()
		.withFile("res/terrain/map/heightmap.bmp")
		.withMipmapping()
		.build();
	m_heightmap->setBinding(t++);
	m_heightmap->setSampler(m_sampler);
	m_shader->bindTexture("heightmap", m_heightmap);

	m_normalmap = device->createTexture()
		.withFile("res/terrain/map/normalmap.bmp")
		.withMipmapping()
		.build();
	m_normalmap->setBinding(t++);
	m_normalmap->setSampler(m_sampler);
	m_shader->bindTexture("normalmap", m_normalmap);
	
	// Material - Grass
	m_materials.push_back(
		Material{
		device->createTexture()
		.withFile("res/terrain/materials/Grass_001/Grass_001_COLOR.jpg")
		.withMipmapping()
		.build(),
		device->createTexture()
		.withFile("res/terrain/materials/Grass_001/Grass_001_DISP.jpg")
		.withMipmapping()
		.build(),
		device->createTexture()
		.withFile("res/terrain/materials/Grass_001/Grass_001_NORM.jpg")
		.withMipmapping()
		.build(),
		device->createTexture()
		.withFile("res/terrain/map/alpha0.bmp")
		.withMipmapping()
		.build() });

	// Material - Dirt
	m_materials.push_back(
		Material{
		device->createTexture()
		.withFile("res/terrain/materials/Sand_002/Sand_002_COLOR.jpg")
		.withMipmapping()
		.build(),
		device->createTexture()
		.withFile("res/terrain/materials/Sand_002/Sand_002_DISP.jpg")
		.withMipmapping()
		.build(),
		device->createTexture()
		.withFile("res/terrain/materials/Sand_002/Sand_002_NRM.jpg")
		.withMipmapping()
		.build(),
		device->createTexture()
		.withFile("res/terrain/map/alpha1.bmp")
		.withMipmapping()
		.build() });

	// Material - Rock 1
	m_materials.push_back(
		Material{
		device->createTexture()
		.withFile("res/terrain/materials/Rough_rock_015/Rough_rock_015_COLOR.jpg")
		.withMipmapping()
		.build(),
		device->createTexture()
		.withFile("res/terrain/materials/Rough_rock_015/Rough_rock_015_DISP.jpg")
		.withMipmapping()
		.build(),
		device->createTexture()
		.withFile("res/terrain/materials/Rough_rock_015/Rough_rock_015_NRM.jpg")
		.withMipmapping()
		.build(),
		device->createTexture()
		.withFile("res/terrain/map/alpha2.bmp")
		.withMipmapping()
		.build() });

	// Material - Rock 2
	m_materials.push_back(
		Material{
		device->createTexture()
		.withFile("res/terrain/materials/Rough_rock_015/Rough_rock_015_COLOR.jpg")
		.withMipmapping()
		.build(),
		device->createTexture()
		.withFile("res/terrain/materials/Rough_rock_015/Rough_rock_015_DISP.jpg")
		.withMipmapping()
		.build(),
		device->createTexture()
		.withFile("res/terrain/materials/Rough_rock_015/Rough_rock_015_NRM.jpg")
		.withMipmapping()
		.build(),
		device->createTexture()
		.withFile("res/terrain/map/alpha3.bmp")
		.withMipmapping()
		.build() });

	for (unsigned i = 0; i < m_materials.size(); ++i) {
		m_materials[i].diffuse->setBinding(t++);
		m_materials[i].mapAlpha->setSampler(m_sampler);

		m_materials[i].displacement->setBinding(t++);
		m_materials[i].displacement->setSampler(m_sampler);

		m_materials[i].normal->setBinding(t++);
		m_materials[i].normal->setSampler(m_sampler);

		m_materials[i].mapAlpha->setBinding(t++);
		m_materials[i].mapAlpha->setSampler(m_sampler);
	}
	
	for (unsigned i = 0; i < m_materials.size(); ++i) {
		m_shader->bindTexture("material" + std::to_string(i) + "Dif", m_materials[i].diffuse);
		m_shader->bindTexture("material" + std::to_string(i) + "Disp", m_materials[i].displacement);
		m_shader->bindTexture("material" + std::to_string(i) + "Norm", m_materials[i].normal);
		m_shader->bindTexture("material" + std::to_string(i) + "Alpha", m_materials[i].mapAlpha);
	}
}

Terrain::~Terrain() {
}

void Terrain::init() {

	// Initialize Root Node
	for (unsigned i = 0; i < rootPatches; ++i) {
		for (unsigned j = 0; j < rootPatches; ++j) {
			m_rootNodes.push_back(
				std::make_unique<TerrainNode>(
					shared_from_this(),
					m_context,
					m_vertexArray,
					m_constantNode,
					glm::vec2(i / (float)rootPatches, j / (float)rootPatches),
					0,
					glm::vec2(i, j)));
		}
	}
}

void Terrain::update() {

	for (unsigned i = 0; i < m_rootNodes.size(); ++i) {
		m_rootNodes[i]->update(m_camera->getPosition());
	}
}

void Terrain::render() {

	// Bind Shader
	m_context->bind(m_shader);

	// Bind Samplers
	m_context->bind(m_sampler, VERTEX_SHADER | DOMAIN_SHADER | GEOMETRY_SHADER | PIXEL_SHADER);

	// Bind Textures
	m_context->bind(m_heightmap, VERTEX_SHADER | DOMAIN_SHADER);
	m_context->bind(m_normalmap, PIXEL_SHADER);

	for (unsigned i = 0; i < m_materials.size(); ++i) {
		m_context->bind(m_materials[i].diffuse, PIXEL_SHADER);
		m_context->bind(m_materials[i].displacement, GEOMETRY_SHADER);
		m_context->bind(m_materials[i].normal, PIXEL_SHADER);
		m_context->bind(m_materials[i].mapAlpha, GEOMETRY_SHADER | PIXEL_SHADER);
	}
	
	// Bind Constants
	m_context->bind(m_constantCamera, GEOMETRY_SHADER | PIXEL_SHADER);
	m_context->bind(m_constantTerrain, VERTEX_SHADER | DOMAIN_SHADER | GEOMETRY_SHADER | PIXEL_SHADER);
	m_context->bind(m_constantNode, VERTEX_SHADER | HULL_SHADER | PIXEL_SHADER);

	// Draw Terrain
	auto type = m_context->getPrimitive();
	m_context->setPrimitive(PATCHES_TYPE, patchSize);
	for (unsigned i = 0; i < m_rootNodes.size(); ++i) {
		m_rootNodes[i]->render();
	}
	m_context->setPrimitive(type);

	// Unbind
	/*
	m_context->unbind(m_constantCamera, GEOMETRY_SHADER | PIXEL_SHADER);
	m_context->unbind(m_constantTerrain, VERTEX_SHADER | DOMAIN_SHADER | GEOMETRY_SHADER | PIXEL_SHADER);
	m_context->unbind(m_constantNode, VERTEX_SHADER | HULL_SHADER | PIXEL_SHADER);

	for (unsigned i = 0; i < m_materials.size(); ++i) {
		m_context->unbind(m_materials[i].diffuse, PIXEL_SHADER);
		m_context->unbind(m_materials[i].displacement, GEOMETRY_SHADER);
		m_context->unbind(m_materials[i].normal, PIXEL_SHADER);
		m_context->unbind(m_materials[i].mapAlpha, GEOMETRY_SHADER | PIXEL_SHADER);
	}

	m_context->unbind(m_heightmap, VERTEX_SHADER | DOMAIN_SHADER);
	m_context->unbind(m_normalmap, PIXEL_SHADER);

	m_context->unbind(m_sampler, VERTEX_SHADER | DOMAIN_SHADER | GEOMETRY_SHADER | PIXEL_SHADER);

	m_context->unbind(m_shader);
	*/
}

const int Terrain::getLodAt(const glm::vec2 pos) {

	int rootI = floor(pos.x * rootPatches);
	int rootJ = floor(pos.y * rootPatches);

	TerrainNode* node = m_rootNodes[rootI * rootPatches + rootJ].get();

	while (!node->m_leaf && node->m_children.size()) {
		glm::vec2 mid = node->m_location + glm::vec2(node->m_size / 2.0f);

		int x = (pos.x < mid.x ? 0 : 1);
		int y = (pos.y < mid.y ? 0 : 1);

		node = node->m_children[x * 2 + y].get();
	}

	return node->m_lod;
}

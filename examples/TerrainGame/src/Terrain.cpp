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
	const std::shared_ptr<ConstantBuffer>& constantCamera)
	: m_context(context)
	, m_camera(camera)
	, m_constantCamera(constantCamera) {
	
	auto inputLayout = InputLayoutConfig().float2();
	auto semanticLayout = SemanticLayoutConfig().position();

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
	m_vertexBuffer = device->createVertexBuffer(
		BufferConfig()
		.asVertexBuffer()
		.setInputLayout(inputLayout)
		.setData(vertices.data(), vertices.size()));

	// Initialize Vertex Array
	m_vertexArray = device->createVertexArray();
	m_vertexArray->addVertexBuffer(m_vertexBuffer);

	// Initialize Samplers
	m_sampler = device->createSampler(
		SamplerConfig()
		.setBinding(0)
		.setMipmap(LINEAR, 0, 8)
		.setFilter(LINEAR)
		.setEdge(WRAP));

	// Load Textures
	unsigned t = 0;
	m_heightmap = device->createTexture(
		TextureConfig()
		.loadFile("res/terrain/map/heightmap.bmp")
		.setBinding(t++)
		.mipmapping());
	m_heightmap->setSampler(m_sampler);

	m_normalmap = device->createTexture(
		TextureConfig()
		.loadFile("res/terrain/map/normalmap.bmp")
		.setBinding(t++)
		.mipmapping());
	m_normalmap->setSampler(m_sampler);
	
	// Material - Grass
	m_materials.push_back(
		Material{
		device->createTexture(
			TextureConfig()
			.loadFile("res/terrain/materials/Grass_001/Grass_001_COLOR.jpg")
			.setBinding(t++)
			.mipmapping()),
		device->createTexture(
			TextureConfig()
			.loadFile("res/terrain/materials/Grass_001/Grass_001_DISP.jpg")
			.setBinding(t++)
			.mipmapping()),
		device->createTexture(
			TextureConfig()
			.loadFile("res/terrain/materials/Grass_001/Grass_001_NORM.jpg")
			.setBinding(t++)
			.mipmapping()),
		device->createTexture(
			TextureConfig()
			.loadFile("res/terrain/map/alpha0.bmp")
			.setBinding(t++)
			.mipmapping()) });

	// Material - Dirt
	m_materials.push_back(
		Material{
		device->createTexture(
			TextureConfig()
			.loadFile("res/terrain/materials/Sand_002/Sand_002_COLOR.jpg")
			.setBinding(t++)
			.mipmapping()),
		device->createTexture(
			TextureConfig()
			.loadFile("res/terrain/materials/Sand_002/Sand_002_DISP.jpg")
			.setBinding(t++)
			.mipmapping()),
		device->createTexture(
			TextureConfig()
			.loadFile("res/terrain/materials/Sand_002/Sand_002_NRM.jpg")
			.setBinding(t++)
			.mipmapping()),
		device->createTexture(
			TextureConfig()
			.loadFile("res/terrain/map/alpha1.bmp")
			.setBinding(t++)
			.mipmapping()) });

	// Material - Rock 1
	m_materials.push_back(
		Material{
		device->createTexture(
			TextureConfig()
			.loadFile("res/terrain/materials/Rough_rock_015/Rough_rock_015_COLOR.jpg")
			.setBinding(t++)
			.mipmapping()),
		device->createTexture(
			TextureConfig()
			.loadFile("res/terrain/materials/Rough_rock_015/Rough_rock_015_DISP.jpg")
			.setBinding(t++)
			.mipmapping()),
		device->createTexture(
			TextureConfig()
			.loadFile("res/terrain/materials/Rough_rock_015/Rough_rock_015_NRM.jpg")
			.setBinding(t++)
			.mipmapping()),
		device->createTexture(
			TextureConfig()
			.loadFile("res/terrain/map/alpha2.bmp")
			.setBinding(t++)
			.mipmapping()) });

	// Material - Rock 2
	m_materials.push_back(
		Material{
		device->createTexture(
			TextureConfig()
			.loadFile("res/terrain/materials/Rough_rock_015/Rough_rock_015_COLOR.jpg")
			.setBinding(t++)
			.mipmapping()),
		device->createTexture(
			TextureConfig()
			.loadFile("res/terrain/materials/Rough_rock_015/Rough_rock_015_DISP.jpg")
			.setBinding(t++)
			.mipmapping()),
		device->createTexture(
			TextureConfig()
			.loadFile("res/terrain/materials/Rough_rock_015/Rough_rock_015_NRM.jpg")
			.setBinding(t++)
			.mipmapping()),
		device->createTexture(
			TextureConfig()
			.loadFile("res/terrain/map/alpha3.bmp")
			.setBinding(t++)
			.mipmapping()) });

	for (unsigned i = 0; i < m_materials.size(); ++i) {
		m_materials[i].diffuse->setSampler(m_sampler);
		m_materials[i].displacement->setSampler(m_sampler);
		m_materials[i].normal->setSampler(m_sampler);
		m_materials[i].mapAlpha->setSampler(m_sampler);
	}

	// Initialize Constant Buffers
	TerrainData terrainData{ glm::mat4(1), scaleXZ, scaleY, detailRangeNear, detailRangeFar };
	terrainData.worldMatrix = glm::translate(terrainData.worldMatrix, glm::vec3(-terrainData.scaleXZ / 2.0f, 0.0f, -terrainData.scaleXZ / 2.0f));
	terrainData.worldMatrix = glm::scale(terrainData.worldMatrix, glm::vec3(terrainData.scaleXZ, terrainData.scaleY, terrainData.scaleXZ));
	for (unsigned i = 0; i < 8; ++i) terrainData.lodRange[i] = lodRange[i];

	m_constantTerrain = device->createConstantBuffer(
		BufferConfig()
		.asConstantBuffer()
		.setBinding(1)
		.setInputLayout(InputLayoutConfig().mat4().float4().float4().float4())
		.setData(&terrainData));

	NodeData nodeData{};
	m_constantNode = device->createConstantBuffer(
		BufferConfig()
		.asConstantBuffer()
		.setBinding(2)
		.setInputLayout(InputLayoutConfig().mat4().float2().float2().float1().int1().float4().float2())
		.setData(&nodeData));
	
	// Initialize Shader
	m_shader = device->createShader(
		ShaderConfig()
		.setGLSLVertexShader("res/terrain/shader/terrain.vs.glsl")
		.setGLSLHullShader("res/terrain/shader/terrain.hs.glsl")
		.setGLSLDomainShader("res/terrain/shader/terrain.ds.glsl")
		.setGLSLGeometryShader("res/terrain/shader/terrain.gs.glsl")
		.setGLSLPixelShader("res/terrain/shader/terrain.ps.glsl")
		.setHLSLVertexShader("res/terrain/shader/terrain.vs.hlsl", "main")
		.setHLSLHullShader("res/terrain/shader/terrain.hs.hlsl", "main")
		.setHLSLDomainShader("res/terrain/shader/terrain.ds.hlsl", "main")
		.setHLSLGeometryShader("res/terrain/shader/terrain.gs.hlsl", "main")
		.setHLSLPixelShader("res/terrain/shader/terrain.ps.hlsl", "main")
		.setInputLayout(inputLayout)
		.setSemanticLayout(semanticLayout));

	m_shader->bindConstant("Camera", m_constantCamera);
	m_shader->bindConstant("Terrain", m_constantTerrain);
	m_shader->bindConstant("Node", m_constantNode);
	m_shader->bindTexture("heightmap", m_heightmap);
	m_shader->bindTexture("normalmap", m_normalmap);

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
	m_shader->bind();

	// Bind Samplers
	m_sampler->bind(VERTEX_SHADER_BIT | DOMAIN_SHADER_BIT | GEOMETRY_SHADER_BIT | PIXEL_SHADER_BIT);

	// Bind Textures
	m_heightmap->bind(VERTEX_SHADER_BIT | DOMAIN_SHADER_BIT);
	m_normalmap->bind(PIXEL_SHADER_BIT);

	for (unsigned i = 0; i < m_materials.size(); ++i) {
		m_materials[i].diffuse->bind(PIXEL_SHADER_BIT);
		m_materials[i].displacement->bind(GEOMETRY_SHADER_BIT);
		m_materials[i].normal->bind(PIXEL_SHADER_BIT);
		m_materials[i].mapAlpha->bind(GEOMETRY_SHADER_BIT | PIXEL_SHADER_BIT);
	}
	
	// Bind Constants
	m_constantCamera->bind(GEOMETRY_SHADER_BIT | PIXEL_SHADER_BIT);
	m_constantTerrain->bind(VERTEX_SHADER_BIT | DOMAIN_SHADER_BIT | GEOMETRY_SHADER_BIT | PIXEL_SHADER_BIT);
	m_constantNode->bind(VERTEX_SHADER_BIT | HULL_SHADER_BIT | PIXEL_SHADER_BIT);

	// Draw Terrain
	auto type = m_context->getPrimitive();
	m_context->setPatchSize(patchSize); // TODO: Reduce to 4
	m_context->setPrimitive(PATCHES_TYPE);
	for (unsigned i = 0; i < m_rootNodes.size(); ++i) {
		m_rootNodes[i]->render();
	}
	m_context->setPrimitive(type);

	// Unbind All
	m_constantNode->unbind(VERTEX_SHADER_BIT | HULL_SHADER_BIT | PIXEL_SHADER_BIT);
	m_constantTerrain->unbind(VERTEX_SHADER_BIT | HULL_SHADER_BIT | DOMAIN_SHADER_BIT | GEOMETRY_SHADER_BIT | PIXEL_SHADER_BIT);
	m_constantCamera->unbind(GEOMETRY_SHADER_BIT | PIXEL_SHADER_BIT);

	for (unsigned i = 0; i < m_materials.size(); ++i) {
		m_materials[i].diffuse->unbind(PIXEL_SHADER_BIT);
		m_materials[i].displacement->unbind(GEOMETRY_SHADER_BIT);
		m_materials[i].normal->unbind(PIXEL_SHADER_BIT);
		m_materials[i].mapAlpha->unbind(GEOMETRY_SHADER_BIT | PIXEL_SHADER_BIT);
	}

	m_normalmap->unbind(PIXEL_SHADER_BIT);
	m_heightmap->unbind(VERTEX_SHADER_BIT | DOMAIN_SHADER_BIT);

	m_sampler->unbind(VERTEX_SHADER_BIT | DOMAIN_SHADER_BIT | GEOMETRY_SHADER_BIT | PIXEL_SHADER_BIT);

	m_shader->unbind();
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

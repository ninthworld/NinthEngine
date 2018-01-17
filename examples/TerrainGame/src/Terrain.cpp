#include <NinthEngine\Utils\FileUtils.hpp>
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
	std::shared_ptr<VertexBuffer> vertexBuffer = device->createVertexBuffer()
		.withLayout(inputLayout)
		.withData(vertices.size(), vertices.data())
		.build();

	// Initialize Vertex Array
	m_vertexArrayTerrain = device->createVertexArray();
	m_vertexArrayTerrain->bind(vertexBuffer);
	
	// Initialize Shader
	m_shaderTerrain = device->createShader()
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

	// Initialize Constant Buffers
	TerrainData terrainData{ glm::mat4(1), scaleXZ, scaleY, detailRangeNear, detailRangeFar };
	terrainData.worldMatrix = glm::translate(terrainData.worldMatrix, glm::vec3(-terrainData.scaleXZ / 2.0f, 0.0f, -terrainData.scaleXZ / 2.0f));
	terrainData.worldMatrix = glm::scale(terrainData.worldMatrix, glm::vec3(terrainData.scaleXZ, terrainData.scaleY, terrainData.scaleXZ));
	for (unsigned i = 0; i < 8; ++i) terrainData.lodRange[i] = lodRange[i];

	m_constantTerrain = device->createConstantBuffer()
		.withLayout(LayoutConfig().float4x4().float4().float4().float4())
		.withData(&terrainData)
		.build();

	NodeData nodeData{};
	m_constantNode = device->createConstantBuffer()
		.withLayout(LayoutConfig().float4x4().float2().float2().float1().int1().float4().float2())
		.withData(&nodeData)
		.build();

	// Initialize Samplers
	m_sampler = device->createSampler()
		.withFilter(LINEAR)
		.withEdge(WRAP)
		.withMipmapping(LINEAR, 0, 8)
		.build();

	// Load Textures
	m_heightmap = device->createTexture()
		.withFile("res/terrain/map/heightmap.bmp")
		.withMipmapping()
		.build();
	m_heightmap->setSampler(m_sampler);

	m_normalmap = device->createTexture()
		.withFile("res/terrain/map/normalmap.bmp")
		.withMipmapping()
		.build();
	m_normalmap->setSampler(m_sampler);

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

	// Bind Constants to Shader
	m_shaderTerrain->bind(0, "Camera", m_constantCamera, GEOMETRY_SHADER | PIXEL_SHADER);
	m_shaderTerrain->bind(1, "Terrain", m_constantTerrain, VERTEX_SHADER | DOMAIN_SHADER | GEOMETRY_SHADER | PIXEL_SHADER);
	m_shaderTerrain->bind(2, "Node", m_constantNode, VERTEX_SHADER | HULL_SHADER | PIXEL_SHADER);

	// Bind Samplers to Shader
	m_shaderTerrain->bind(0, "texSampler", m_sampler, VERTEX_SHADER | DOMAIN_SHADER | GEOMETRY_SHADER | PIXEL_SHADER);

	// Bind Textures to Shader
	unsigned t = 0;
	m_shaderTerrain->bind(t++, "heightmap", m_heightmap, VERTEX_SHADER | DOMAIN_SHADER);
	m_shaderTerrain->bind(t++, "normalmap", m_normalmap, PIXEL_SHADER);

	for (unsigned i = 0; i < m_materials.size(); ++i) {
		m_materials[i].diffuse->setSampler(m_sampler);
		m_materials[i].displacement->setSampler(m_sampler);
		m_materials[i].normal->setSampler(m_sampler);
		m_materials[i].mapAlpha->setSampler(m_sampler);

		m_shaderTerrain->bind(t++, "material" + std::to_string(i) + "Dif", m_materials[i].diffuse, PIXEL_SHADER);
		m_shaderTerrain->bind(t++, "material" + std::to_string(i) + "Disp", m_materials[i].displacement, GEOMETRY_SHADER);
		m_shaderTerrain->bind(t++, "material" + std::to_string(i) + "Norm", m_materials[i].normal, PIXEL_SHADER);
		m_shaderTerrain->bind(t++, "material" + std::to_string(i) + "Alpha", m_materials[i].mapAlpha, GEOMETRY_SHADER | PIXEL_SHADER);
	}

	// Grass
	m_blender = device->createBlender()
		.withAlphaToCoverage()
		.build();

	m_rasterizerNoCull = device->createRasterizer()
		.withFill()
		.withDepthClipping()
		.withMultisampling()
		.withFrontCCW()
		.build();

	m_constantGrass = device->createConstantBuffer()
		.withLayout(LayoutConfig().float4())
		.withData((void*)glm::value_ptr(glm::vec4(m_timeStep)))
		.build();

	inputLayout = LayoutConfig().float3(POSITION).float3(NORMAL).float2(TEXCOORD);

	struct Vertex { glm::vec3 position, normal; glm::vec2 texCoord; };
	std::vector<Vertex> grassVertices{
		{ glm::vec3(0, 0, -1), glm::vec3(0, 1, 0), glm::vec2(0, 1) },
		{ glm::vec3(0, 0, 1), glm::vec3(0, 1, 0), glm::vec2(1, 1) },
		{ glm::vec3(0, 2, 1), glm::vec3(0, 1, 0), glm::vec2(1, 0) },
		{ glm::vec3(0, 2, -1), glm::vec3(0, 1, 0), glm::vec2(0, 0) },

		{ glm::vec3(cos(PI / 6), 0, -sin(PI / 6)), glm::vec3(0, 1, 0), glm::vec2(0, 1) },
		{ glm::vec3(-cos(PI / 6), 0, sin(PI / 6)), glm::vec3(0, 1, 0), glm::vec2(1, 1) },
		{ glm::vec3(-cos(PI / 6), 2, sin(PI / 6)), glm::vec3(0, 1, 0), glm::vec2(1, 0) },
		{ glm::vec3(cos(PI / 6), 2, -sin(PI / 6)), glm::vec3(0, 1, 0), glm::vec2(0, 0) },

		{ glm::vec3(cos(PI / 6), 0, sin(PI / 6)), glm::vec3(0, 1, 0), glm::vec2(0, 1) },
		{ glm::vec3(-cos(PI / 6), 0, -sin(PI / 6)), glm::vec3(0, 1, 0), glm::vec2(1, 1) },
		{ glm::vec3(-cos(PI / 6), 2, -sin(PI / 6)), glm::vec3(0, 1, 0), glm::vec2(1, 0) },
		{ glm::vec3(cos(PI / 6), 2, sin(PI / 6)), glm::vec3(0, 1, 0), glm::vec2(0, 0) }
	};

	std::vector<short> indices{
		0, 1, 2, 2, 3, 0,
		4, 5, 6, 6, 7, 4,
		8, 9, 10, 10, 11, 8
	};

	std::shared_ptr<VertexBuffer> vertexBufferGrass = device->createVertexBuffer()
		.withLayout(inputLayout)
		.withData(grassVertices.size(), grassVertices.data())
		.build();

	m_indexBufferGrass = device->createIndexBuffer()
		.withLayout(LayoutConfig().short1())
		.withData(indices.size(), indices.data())
		.build();

	m_vertexArrayGrass = device->createVertexArray();
	m_vertexArrayGrass->bind(vertexBufferGrass);

	m_shaderGrass = device->createShader()
		.withLayout(inputLayout)
		.withGLSL<VERTEX_SHADER>("res/grass/grass.vs.glsl")
		.withGLSL<PIXEL_SHADER>("res/grass/grass.ps.glsl")
		.withHLSL<VERTEX_SHADER>("res/grass/grass.vs.hlsl", "main")
		.withHLSL<PIXEL_SHADER>("res/grass/grass.ps.hlsl", "main")
		.build();

	m_grassColor = device->createTexture()
		.withFile("res/grass/textures/grass_color.bmp")
		.withMipmapping()
		.build();
	m_grassColor->setSampler(m_sampler);

	m_grassAlpha = device->createTexture()
		.withFile("res/grass/textures/grass_alpha.bmp")
		.withMipmapping()
		.build();
	m_grassAlpha->setSampler(m_sampler);

	m_shaderGrass->bind(0, "Camera", m_constantCamera);
	m_shaderGrass->bind(1, "Terrain", m_constantTerrain);
	m_shaderGrass->bind(2, "Node", m_constantNode);
	m_shaderGrass->bind(3, "Grass", m_constantGrass);

	m_shaderGrass->bind(0, "texSampler", m_sampler, VERTEX_SHADER | PIXEL_SHADER);

	m_shaderGrass->bind(0, "grassColor", m_grassColor);
	m_shaderGrass->bind(1, "grassAlpha", m_grassAlpha);
	m_shaderGrass->bind(2, "heightmap", m_heightmap, VERTEX_SHADER);
	m_shaderGrass->bind(3, "normalmap", m_normalmap, VERTEX_SHADER);
	m_shaderGrass->bind(4, "alphamap", m_materials[0].mapAlpha, VERTEX_SHADER);

	// Initialize Height Data
	unsigned char* data = stbi_load("res/terrain/map/heightmap.bmp", &m_heightWidth, &m_heightHeight, nullptr, 1);
	m_heightData = (float*)malloc(m_heightWidth * m_heightHeight * sizeof(float));
	for (unsigned i = 0; i < m_heightWidth; ++i) {
		for (unsigned j = 0; j < m_heightHeight; ++j) {
			unsigned index = i * m_heightHeight + j;
			m_heightData[index] = (data[index] / 255.0f);
		}
	}
	stbi_image_free(data);
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
					glm::vec2(i / (float)rootPatches, j / (float)rootPatches),
					0,
					glm::vec2(i, j)));
		}
	}
}

void Terrain::update() {

	for (unsigned i = 0; i < m_rootNodes.size(); ++i) {
		m_rootNodes[i]->update();
	}

#ifdef BIND_CAMERA_TO_GROUND
	glm::vec3 camPos = m_camera->getPosition();
	float height = getIntHeightAt(glm::vec2(camPos.x, camPos.z));
	m_camera->setPosition(glm::vec3(camPos.x, 8 + height, camPos.z));
#endif

	// Grass
	m_timeStep += 0.001f;
	m_context->setData(m_constantGrass, (void*)glm::value_ptr(glm::vec4(m_timeStep)));
}

void Terrain::render() {

	for (unsigned i = 0; i < m_rootNodes.size(); ++i) {
		m_rootNodes[i]->preRender();
	}
	
	// Bind Terrain Shader
	m_context->bind(m_shaderTerrain);

	// Draw Terrain
	m_context->bind(m_vertexArrayTerrain);

	auto type = m_context->getPrimitive();
	m_context->setPrimitive(PATCHES_TYPE, patchSize);
	for (unsigned i = 0; i < m_rootNodes.size(); ++i) {
		m_rootNodes[i]->renderTerrain();
	}

	m_context->setPrimitive(type);

	m_context->unbind(m_shaderTerrain);

	m_context->bind(m_blender);
	m_context->bind(m_rasterizerNoCull);
	m_context->bind(m_shaderGrass);
	m_context->bind(m_vertexArrayGrass);
	for (unsigned i = 0; i < m_rootNodes.size(); ++i) {
		m_rootNodes[i]->renderGrass();
	}
	m_context->unbind(m_shaderGrass);
	m_context->unbind(m_blender);
}

const int Terrain::getLodAt(const glm::vec2 localPos) {

	int rootI = floor(localPos.x * rootPatches);
	int rootJ = floor(localPos.y * rootPatches);

	TerrainNode* node = m_rootNodes[rootI * rootPatches + rootJ].get();

	while (!node->m_leaf && node->m_children.size()) {
		glm::vec2 mid = node->m_location + glm::vec2(node->m_size / 2.0f);

		int x = (localPos.x < mid.x ? 0 : 1);
		int y = (localPos.y < mid.y ? 0 : 1);

		node = node->m_children[x * 2 + y].get();
	}

	return node->m_lod;
}

const float Terrain::getIntHeightAt(const glm::vec2 worldPos) {

	// Bilinear Interpolation Algorithm
	float x = m_heightWidth * ((worldPos.y + scaleXZ / 2) / scaleXZ);
	float y = m_heightHeight * ((worldPos.x + scaleXZ / 2) / scaleXZ);
	
	int x1 = floor(x);
	int x2 = ceil(x);
	int y1 = floor(y);
	int y2 = ceil(y);

	float q11 = m_heightData[x1 * m_heightHeight + y1];
	float q12 = m_heightData[x1 * m_heightHeight + y2];
	float q21 = m_heightData[x2 * m_heightHeight + y1];
	float q22 = m_heightData[x2 * m_heightHeight + y2];

	float r1 = q11 * (x2 - x) / (x2 - x1) + q21 * (x - x1) / (x2 - x1);
	float r2 = q12 * (x2 - x) / (x2 - x1) + q22 * (x - x1) / (x2 - x1);

	return (r1 * (y2 - y) / (y2 - y1) + r2 * (y - y1) / (y2 - y1)) * scaleY;
}

const float Terrain::getHeightAt(const glm::vec2 localPos) {
	int x = floor(localPos.x * m_heightWidth);
	int y = floor(localPos.y * m_heightHeight);

	return m_heightData[x * m_heightHeight + y] * scaleY;
}

const float Terrain::getMaxHeightAt(const glm::vec2 localPos0, const glm::vec2 localPos1) {
	glm::vec2 p0 = glm::vec2(localPos0.y, localPos0.x);
	glm::vec2 p1 = glm::vec2(localPos1.y, localPos1.x);

	int x = floor(p0.x * m_heightWidth);
	int y = floor(p0.y * m_heightHeight);
	int dX = floor(abs(p1.x - p0.x) * m_heightWidth);
	int dY = floor(abs(p1.y - p0.y) * m_heightHeight);

	float maxHeight = 0;
	unsigned size = m_heightWidth * m_heightHeight;
	for (unsigned i = 0; i < dX; ++i) {
		for (unsigned j = 0; j < dY; ++j) {
			unsigned index = std::min((x + i) * m_heightHeight + (y + i), size - 1);
			maxHeight = std::max(maxHeight, m_heightData[index] * scaleY);
		}
	}
	return maxHeight;
}
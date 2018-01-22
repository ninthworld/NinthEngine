#include "TerrainNode.hpp"
#include "Terrain.hpp"
#include <NinthEngine\Utils\LodePNG\lodepng.h>

Terrain::Terrain(
	const std::shared_ptr<GraphicsDevice>& device,
	const std::shared_ptr<GraphicsContext>& context,
	const std::shared_ptr<GameCamera>& camera,
	const std::shared_ptr<ConstantBuffer>& constantCamera,
	const std::shared_ptr<ConstantBuffer>& constantShadowMap,
	const TerrainConfig config)
	: m_context(context)
	, m_camera(camera)
	, m_config(config) {

	initSamplers(device);
	initGeometry(device);
	initConstants(device);
	initTextures(device);
	initShaders(device, constantCamera, constantShadowMap);
}

void Terrain::initSamplers(const std::shared_ptr<GraphicsDevice>& device) {
	m_sampler = device->createSampler()
		.withEdge(WRAP)
		.withFilter(LINEAR)
		.withMipmapping(LINEAR, 0, 8)
		.build();	
}

void Terrain::initGeometry(const std::shared_ptr<GraphicsDevice>& device) {

	unsigned divs = m_config.nodeVertexCount;
	std::vector<glm::vec2> vertices;
	for (unsigned i = 0; i <= divs; ++i) {
		for (unsigned j = 0; j <= divs; ++j) {
			vertices.push_back(glm::vec2(
				i / (float)divs,
				j / (float)divs));
		}
	}

	m_vertexArray = device->createVertexArray();
	m_vertexArray->bind(
		device->createVertexBuffer()
		.withLayout(terrainLayout)
		.withData(vertices.size(), vertices.data())
		.build());

	for (unsigned k = 0; k < 16; ++k) {
		std::vector<short> indices;
		// Middle indices
		for (unsigned i = 0; i < divs; i += 2) {
			for (unsigned j = 0; j < divs; j += 2) {
				if (k & NORTH && i == 0) {
					indices.push_back(i * (divs + 1) + (j + 2));
					indices.push_back((i + 1) * (divs + 1) + (j + 1));
					indices.push_back(i * (divs + 1) + j);
				}
				else {
					indices.push_back(i * (divs + 1) + (j + 1));
					indices.push_back((i + 1) * (divs + 1) + (j + 1));
					indices.push_back(i * (divs + 1) + j);

					indices.push_back(i * (divs + 1) + (j + 2));
					indices.push_back((i + 1) * (divs + 1) + (j + 1));
					indices.push_back(i * (divs + 1) + (j + 1));
				}

				if (k & SOUTH && i == divs - 2) {
					indices.push_back((i + 2) * (divs + 1) + j);
					indices.push_back((i + 1) * (divs + 1) + (j + 1));
					indices.push_back((i + 2) * (divs + 1) + (j + 2));
				}
				else {
					indices.push_back((i + 2) * (divs + 1) + j);
					indices.push_back((i + 1) * (divs + 1) + (j + 1));
					indices.push_back((i + 2) * (divs + 1) + (j + 1));

					indices.push_back((i + 2) * (divs + 1) + (j + 1));
					indices.push_back((i + 1) * (divs + 1) + (j + 1));
					indices.push_back((i + 2) * (divs + 1) + (j + 2));
				}

				if (k & EAST && j == divs - 2) {
					indices.push_back((i + 2) * (divs + 1) + (j + 2));
					indices.push_back((i + 1) * (divs + 1) + (j + 1));
					indices.push_back(i * (divs + 1) + (j + 2));
				}
				else {
					indices.push_back((i + 1) * (divs + 1) + (j + 2));
					indices.push_back((i + 1) * (divs + 1) + (j + 1));
					indices.push_back(i * (divs + 1) + (j + 2));

					indices.push_back((i + 2) * (divs + 1) + (j + 2));
					indices.push_back((i + 1) * (divs + 1) + (j + 1));
					indices.push_back((i + 1) * (divs + 1) + (j + 2));
				}

				if (k & WEST && j == 0) {
					indices.push_back(i * (divs + 1) + j);
					indices.push_back((i + 1) * (divs + 1) + (j + 1));
					indices.push_back((i + 2) * (divs + 1) + j);
				}
				else {
					indices.push_back(i * (divs + 1) + j);
					indices.push_back((i + 1) * (divs + 1) + (j + 1));
					indices.push_back((i + 1) * (divs + 1) + j);

					indices.push_back((i + 1) * (divs + 1) + j);
					indices.push_back((i + 1) * (divs + 1) + (j + 1));
					indices.push_back((i + 2) * (divs + 1) + j);
				}
			}
		}

		m_indexBuffers[k] = device->createIndexBuffer()
			.withLayout(LayoutConfig().short1())
			.withData(indices.size(), indices.data())
			.build();
	}
}

void Terrain::initConstants(const std::shared_ptr<GraphicsDevice>& device) {

	auto terrainStruct = TerrainStruct{ glm::vec4(m_config.scale, 1) };
	m_constantTerrain = device->createConstantBuffer()
		.withLayout(terrainStructLayout)
		.withData(&terrainStruct)
		.build();

	auto nodeStruct = NodeStruct{ glm::vec2(0, 0), 0, 0 };
	m_constantNode = device->createConstantBuffer()
		.withLayout(nodeStructLayout)
		.withData(&nodeStruct)
		.build();
}

void Terrain::initTextures(const std::shared_ptr<GraphicsDevice>& device) {

	std::vector<unsigned char> image;
	unsigned width, height;
	lodepng::decode(image, width, height, "res/terrain/maps/heightmap.png", LodePNGColorType::LCT_GREY, 16);

	for (unsigned i = 0; i < width * height * 2; i += 2) {
		unsigned char t = image[i];
		image[i] = image[i + 1];
		image[i + 1] = t;
	}

	m_heightmap = device->createTexture()
		.withFormat(FORMAT_R_16_UNORM)
		.withSize(width, height)
		.build();
	m_heightmap->setSampler(m_sampler);
	m_context->setData(m_heightmap, &image[0]);

	// Load Normalmap
	m_normalmap = device->createTexture()
		.withFile("res/terrain/maps/normalmap.png")
		.build();
	m_normalmap->setSampler(m_sampler);

	// Load Splatmap
	m_splatmap = device->createTexture()
		.withFile("res/terrain/maps/splatmap.png")
		.build();
	m_splatmap->setSampler(m_sampler);

	// Load materials
	m_materials.push_back(Material{
		device->createTexture().withFile("res/terrain/textures/grass/grass01_color.bmp").withMipmapping().build(),
		device->createTexture().withFile("res/terrain/textures/grass/grass01_norm.bmp").withMipmapping().build() });

	m_materials.push_back(Material{
		device->createTexture().withFile("res/terrain/textures/dirt/dirt01_color.bmp").withMipmapping().build(),
		device->createTexture().withFile("res/terrain/textures/dirt/dirt01_norm.bmp").withMipmapping().build() });

	m_materials.push_back(Material{
		device->createTexture().withFile("res/terrain/textures/dirt/dirt02_color.bmp").withMipmapping().build(),
		device->createTexture().withFile("res/terrain/textures/dirt/dirt02_norm.bmp").withMipmapping().build() });

	m_materials.push_back(Material{
		device->createTexture().withFile("res/terrain/textures/stone/stone01_color.bmp").withMipmapping().build(),
		device->createTexture().withFile("res/terrain/textures/stone/stone01_norm.bmp").withMipmapping().build() });

	m_materials.push_back(Material{
		device->createTexture().withFile("res/terrain/textures/stone/stone02_color.bmp").withMipmapping().build(),
		device->createTexture().withFile("res/terrain/textures/stone/stone02_norm.bmp").withMipmapping().build() });

	for (unsigned i = 0; i < m_materials.size(); ++i) {
		m_materials[i].color->setSampler(m_sampler);
		m_materials[i].normal->setSampler(m_sampler);
	}
}

void Terrain::initShaders(
	const std::shared_ptr<GraphicsDevice>& device,
	const std::shared_ptr<ConstantBuffer>& constantCamera,
	const std::shared_ptr<ConstantBuffer>& constantShadowMap) {

	m_shader = device->createShader()
		.withLayout(terrainLayout)
		.withGLSL<VERTEX_SHADER>("res/terrain/shaders/glsl/terrain.vs.glsl")
		.withGLSL<GEOMETRY_SHADER>("res/terrain/shaders/glsl/terrain.gs.glsl")
		.withGLSL<PIXEL_SHADER>("res/terrain/shaders/glsl/terrain.ps.glsl")
		.withHLSL<VERTEX_SHADER>("res/terrain/shaders/hlsl/terrain.vs.hlsl", "main")
		.withHLSL<GEOMETRY_SHADER>("res/terrain/shaders/hlsl/terrain.gs.hlsl", "main")
		.withHLSL<PIXEL_SHADER>("res/terrain/shaders/hlsl/terrain.ps.hlsl", "main")
		.build();

	m_shader->bind(0, "texSampler", m_sampler, VERTEX_SHADER | PIXEL_SHADER);

	m_shader->bind(0, "Camera", constantCamera, GEOMETRY_SHADER | PIXEL_SHADER);
	m_shader->bind(1, "Terrain", m_constantTerrain, VERTEX_SHADER | GEOMETRY_SHADER | PIXEL_SHADER);
	m_shader->bind(2, "Node", m_constantNode, VERTEX_SHADER);

	unsigned t = 0;
	m_shader->bind(t++, "heightmap", m_heightmap, VERTEX_SHADER);
	m_shader->bind(t++, "normalmap", m_normalmap, PIXEL_SHADER);
	m_shader->bind(t++, "splatmap", m_normalmap, PIXEL_SHADER);

	for (unsigned i = 0; i < m_materials.size(); ++i) {
		m_shader->bind(t++, "material" + std::to_string(i) + "_color", m_materials[i].color, PIXEL_SHADER);
		m_shader->bind(t++, "material" + std::to_string(i) + "_normal", m_materials[i].normal, PIXEL_SHADER);
	}

	m_shaderShadow = device->createShader()
		.withLayout(terrainLayout)
		.withGLSL<VERTEX_SHADER>("res/terrain/shaders/glsl/shadow/terrainShadow.vs.glsl")
		.withGLSL<PIXEL_SHADER>("res/terrain/shaders/glsl/shadow/terrainShadow.ps.glsl")
		.withHLSL<VERTEX_SHADER>("res/terrain/shaders/hlsl/shadow/terrainShadow.vs.hlsl", "main")
		.withHLSL<PIXEL_SHADER>("res/terrain/shaders/hlsl/shadow/terrainShadow.ps.hlsl", "main")
		.build();
	
	m_shaderShadow->bind(0, "Shadow", constantShadowMap, VERTEX_SHADER);
	m_shaderShadow->bind(1, "Terrain", m_constantTerrain, VERTEX_SHADER);
	m_shaderShadow->bind(2, "Node", m_constantNode, VERTEX_SHADER);
	m_shaderShadow->bind(0, "texSampler", m_sampler, VERTEX_SHADER);
	m_shaderShadow->bind(0, "heightmap", m_heightmap, VERTEX_SHADER);
}

Terrain::~Terrain() {
}

void Terrain::init() {

	float size = 1 / float(m_config.rootCount);
	for (unsigned i = 0; i < m_config.rootCount; ++i) {
		for (unsigned j = 0; j < m_config.rootCount; ++j) {
			m_children.push_back(std::make_shared<TerrainNode>(
				shared_from_this(),
				size,
				glm::vec2(i * size, j * size),
				glm::vec2(i, j),
				0));
		}
	}
}

void Terrain::update() {
	for (unsigned i = 0; i < m_children.size(); ++i) {
		m_children[i]->update();
	}
}

void Terrain::preRender() {
	for (unsigned i = 0; i < m_children.size(); ++i) {
		m_children[i]->preRender();
	}
}

void Terrain::render() {

	m_context->bind(m_shader);
	m_context->bind(m_vertexArray);
	for (unsigned i = 0; i < m_children.size(); ++i) {
		m_children[i]->render();
	}
	m_context->unbind(m_vertexArray);
	m_context->unbind(m_shader);
}

void Terrain::renderShadows() {

	m_context->bind(m_shaderShadow);
	m_context->bind(m_vertexArray);
	for (unsigned i = 0; i < m_children.size(); ++i) {
		m_children[i]->render();
	}
	m_context->unbind(m_vertexArray);
	m_context->unbind(m_shaderShadow);
}

void Terrain::onKeyboard(const Key key, const KeyState state) {
}

const int Terrain::getLodAt(const glm::vec2 localPos) {

	int rootI = floor(localPos.x * m_config.rootCount);
	int rootJ = floor(localPos.y * m_config.rootCount);

	if (rootI * m_config.rootCount + rootJ < m_children.size()) {

		TerrainNode* node = m_children[rootI * m_config.rootCount + rootJ].get();

		while (!node->m_leaf && node->m_childCount == 4) {
			glm::vec2 mid = node->m_localPos + glm::vec2(node->m_localSize / 2.0f);

			int x = (localPos.x < mid.x ? 0 : 1);
			int y = (localPos.y < mid.y ? 0 : 1);

			node = node->m_children[x * 2 + y].get();
		}

		return node->m_lod;
	}
	else {
		return 0;
	}
}
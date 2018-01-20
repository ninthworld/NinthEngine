#include "TerrainNode.hpp"
#include "Terrain.hpp"

Terrain::Terrain(
	const std::shared_ptr<GraphicsDevice>& device,	
	const std::shared_ptr<GraphicsContext>& context,
	const std::shared_ptr<GameCamera>& camera,
	const std::shared_ptr<ConstantBuffer>& constantCamera,
	const TerrainConfig config)
	: m_context(context)
	, m_camera(camera)
	, m_constantCamera(constantCamera)
	, m_config(config) {

	// Initialize Vertex Geometry
	auto layout = LayoutConfig().float2(POSITION);
	unsigned divs = m_config.nodeVertexCount;
	std::vector<glm::vec2> vertices;
	for (unsigned i = 0; i < divs; ++i) {
		for (unsigned j = 0; j < divs; ++j) {
			vertices.push_back(glm::vec2(
				i / float(divs - 1),
				j / float(divs - 1)));
		}
	}

	m_vertexArray = device->createVertexArray();
	m_vertexArray->bind(
		device->createVertexBuffer()
		.withLayout(layout)
		.withData(vertices.size(), vertices.data())
		.build());

	// Initialize Constants
	auto terrainStruct = TerrainStruct{ glm::vec4(m_config.scale, 1) };
	m_constantTerrain = device->createConstantBuffer()
		.withLayout(LayoutConfig().float4())
		.withData(&terrainStruct)
		.build();

	auto nodeStruct = NodeStruct{ glm::vec2(0, 0), 0, 0, glm::vec4(0) };
	m_constantNode = device->createConstantBuffer()
		.withLayout(LayoutConfig().float2().float1().int1().float4())
		.withData(&nodeStruct)
		.build();

	// Initialize Shader
	m_shader = device->createShader()
		.withLayout(layout)
		.withGLSL<VERTEX_SHADER>("res/terrain/shaders/glsl/terrain.vs.glsl")
		.withGLSL<HULL_SHADER>("res/terrain/shaders/glsl/terrain.hs.glsl")
		.withGLSL<DOMAIN_SHADER>("res/terrain/shaders/glsl/terrain.ds.glsl")
		.withGLSL<GEOMETRY_SHADER>("res/terrain/shaders/glsl/terrain.gs.glsl")
		.withGLSL<PIXEL_SHADER>("res/terrain/shaders/glsl/terrain.ps.glsl")
		.build();

	// Initialize Textures
	m_sampler = device->createSampler()
		.withEdge(WRAP)
		.withFilter(LINEAR)
		.withMipmapping(LINEAR, 0, 8)
		.build();
	m_shader->bind(0, "texSampler", m_sampler, VERTEX_SHADER | PIXEL_SHADER);

	m_heightmap = device->createTexture()
		.withFile("res/terrain/maps/heightmap.bmp")
		.build();
	m_normalmap = device->createTexture()
		.withFile("res/terrain/maps/normalmap.bmp")
		.build();
	m_heightmap->setSampler(m_sampler);
	m_normalmap->setSampler(m_sampler);

	m_fractal01_heightmap = device->createTexture()
		.withFile("res/terrain/maps/fractal01_heightmap.bmp")
		.build();
	m_fractal01_normalmap = device->createTexture()
		.withFile("res/terrain/maps/fractal01_normalmap.bmp")
		.build();
	m_fractal01_heightmap->setSampler(m_sampler);
	m_fractal01_normalmap->setSampler(m_sampler);

	m_fractal02_heightmap = device->createTexture()
		.withFile("res/terrain/maps/fractal02_heightmap.bmp")
		.build();
	m_fractal02_normalmap = device->createTexture()
		.withFile("res/terrain/maps/fractal02_normalmap.bmp")
		.build();
	m_fractal02_heightmap->setSampler(m_sampler);
	m_fractal02_normalmap->setSampler(m_sampler);

	m_blobmap = device->createTexture()
		.withFile("res/terrain/maps/blobmap.bmp")
		.build();
	m_blobmap->setSampler(m_sampler);

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
	
	m_shader->bind(0, "Camera", m_constantCamera);
	m_shader->bind(1, "Terrain", m_constantTerrain);
	m_shader->bind(2, "Node", m_constantNode);

	unsigned t = 0;
	m_shader->bind(t++, "heightmap", m_heightmap, DOMAIN_SHADER);
	m_shader->bind(t++, "normalmap", m_normalmap, PIXEL_SHADER);
	m_shader->bind(t++, "blobmap", m_blobmap, PIXEL_SHADER);
	m_shader->bind(t++, "fractal01_heightmap", m_fractal01_heightmap, DOMAIN_SHADER);
	m_shader->bind(t++, "fractal01_normalmap", m_fractal01_normalmap, PIXEL_SHADER);
	m_shader->bind(t++, "fractal01_heightmap", m_fractal02_heightmap, DOMAIN_SHADER);
	m_shader->bind(t++, "fractal01_normalmap", m_fractal02_normalmap, PIXEL_SHADER);

	for (unsigned i = 0; i < m_materials.size(); ++i) {
		m_materials[i].color->setSampler(m_sampler);
		m_materials[i].normal->setSampler(m_sampler);

		m_shader->bind(t++, "material" + std::to_string(i) + "_color", m_materials[i].color, PIXEL_SHADER);
		m_shader->bind(t++, "material" + std::to_string(i) + "_normal", m_materials[i].normal, PIXEL_SHADER);
	}
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

void Terrain::render() {

	for (unsigned i = 0; i < m_children.size(); ++i) {
		m_children[i]->preRender();
	}

	m_context->bind(m_shader);
	m_context->bind(m_vertexArray);
	
	auto primitive = m_context->getPrimitive();
	m_context->setPrimitive(PATCHES_TYPE, pow(m_config.nodeVertexCount, 2));
	
	for (unsigned i = 0; i < m_children.size(); ++i) {
		m_children[i]->render();
	}
	
	m_context->setPrimitive(primitive);

	m_context->unbind(m_vertexArray);
	m_context->unbind(m_shader);
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
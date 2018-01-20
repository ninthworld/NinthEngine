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
	, m_config(config)
	, m_wireframe(false) {

	// Initialize Rasterizers
	m_rasterizerDefault = device->createRasterizer()
		.withFill()
		.withDepthClipping()
		.withMultisampling()
		.withCull()
		.withFrontCCW()
		.build();
	m_context->bind(m_rasterizerDefault);

	m_rasterizerWireframe = device->createRasterizer()
		.withFill(WIREFRAME)
		.withDepthClipping()
		.withMultisampling()
		.withFrontCCW()
		.build();

	// Initialize Constants
	auto terrainStruct = TerrainStruct{ glm::vec4(m_config.scale, 1) };
	m_constantTerrain = device->createConstantBuffer()
		.withLayout(LayoutConfig().float4())
		.withData(&terrainStruct)
		.build();

	auto nodeStruct = NodeStruct{ glm::vec2(0, 0), 0, 0 };
	m_constantNode = device->createConstantBuffer()
		.withLayout(LayoutConfig().float2().float1().int1())
		.withData(&nodeStruct)
		.build();

	// Initialize Shader
	auto layout = LayoutConfig().float2(POSITION);
	m_shader = device->createShader()
		.withLayout(layout)
		.withGLSL<VERTEX_SHADER>("res/terrain/shaders/terrain.vs.glsl")
		.withGLSL<PIXEL_SHADER>("res/terrain/shaders/terrain.ps.glsl")
		.build();
	m_shader->bind(0, "Camera", m_constantCamera);
	m_shader->bind(1, "Terrain", m_constantTerrain);
	m_shader->bind(2, "Node", m_constantNode);

	// Initialize Vertex Geometry
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
			.withLayout(layout)
			.withData(vertices.size(), vertices.data())
			.build());

	// Initialize Index Geometry
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

	// Initialize Textures
	m_sampler = device->createSampler()
		.withFilter(LINEAR)
		.withMipmapping(LINEAR, 0, 8)
		.build();
	m_shader->bind(0, "texSampler", m_sampler, VERTEX_SHADER | PIXEL_SHADER);

	m_heightmap = device->createTexture()
		.withFile("res/heightmap.bmp")
		.build();

	m_normalmap = device->createTexture()
		.withFile("res/normalmap.bmp")
		.build();

	m_shader->bind(0, "heightmap", m_heightmap, VERTEX_SHADER);
	m_shader->bind(1, "normalmap", m_normalmap, PIXEL_SHADER);
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

	if (m_wireframe) m_context->bind(m_rasterizerWireframe);
	else m_context->bind(m_rasterizerDefault);

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
	for (unsigned i = 0; i < m_children.size(); ++i) {
		m_children[i]->render();
	}
	m_context->unbind(m_vertexArray);
	m_context->unbind(m_shader);
}

void Terrain::onKeyboard(const Key key, const KeyState state) {
	if (key == KEY_1 && state == KS_RELEASED) {
		m_wireframe = !m_wireframe;
	}
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
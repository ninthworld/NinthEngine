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
	const std::shared_ptr<GameCamera>& camera)
	: m_context(context)
	, m_camera(camera) {
	
	// Load Textures
	m_textureHeight = device->createTexture(
		TextureConfig()
		.loadFile("res/terrain/textures/heightmap2.png")
		.setBinding(0));

	m_textureNormal = device->createTexture(
		TextureConfig()
		.loadFile("res/terrain/textures/normalmap2.png")
		.setBinding(1));

	m_textureRockDiffuse = device->createTexture(
		TextureConfig()
		.loadFile("res/terrain/textures/Rough_rock/Rough_rock_004_COLOR.png")
		.setBinding(2));

	m_textureRockNormal = device->createTexture(
		TextureConfig()
		.loadFile("res/terrain/textures/Rough_rock/Rough_rock_004_NRM.png")
		.setBinding(3));

	// Initialize Constants Buffer
	m_constantsViewProj = device->createConstantsBuffer(
		BufferConfig()
		.asConstantsBuffer()
		.setBinding(0)
		.setInputLayout(InputLayoutConfig().mat4())
		.setData(glm::mat4(1)));

	m_constantsModel = device->createConstantsBuffer(
		BufferConfig()
		.asConstantsBuffer()
		.setBinding(1)
		.setInputLayout(InputLayoutConfig().mat4())
		.setData(glm::mat4(1)));

	// Load Shader
	auto inputLayout = InputLayoutConfig().float2();
	auto semanticLayout = SemanticLayoutConfig().position();

	m_shader = device->createShader(
		ShaderConfig()
		.setGLSLVertexShader("res/terrain/shaders/terrain.vs.glsl")
		.setGLSLPixelShader("res/terrain/shaders/terrain.ps.glsl")
		.setHLSLVertexShader("res/terrain/shaders/terrain.vs.hlsl", "main")
		.setHLSLPixelShader("res/terrain/shaders/terrain.ps.hlsl", "main")
		.setInputLayout(inputLayout)
		.setSemanticLayout(semanticLayout));

	// Bind Buffers to Shader
	m_shader->bindConstants("ViewProjMatrix", m_constantsViewProj);
	m_shader->bindConstants("ModelMatrix", m_constantsModel);
	m_shader->bindTexture("heightTexture", m_textureHeight);
	m_shader->bindTexture("normalTexture", m_textureNormal);
	m_shader->bindTexture("rockDiffuseTexture", m_textureRockDiffuse);
	m_shader->bindTexture("rockNormalTexture", m_textureRockNormal);

	// Generate Model Data
	std::vector<glm::vec2> vertices;

	const unsigned divs = 8;
	for (unsigned i = 0; i <= divs; ++i) {
		for (unsigned j = 0; j <= divs; ++j) {
			vertices.push_back(glm::vec2(i / (float)divs, j / (float)divs) - glm::vec2(0.5f, 0.5f));
		}
	}
	
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

		// Initialize Index Buffer
		m_indexBuffers.push_back(
			device->createIndexBuffer(
				BufferConfig()
				.asIndexBuffer()
				.setInputLayout(InputLayoutConfig().short1())
				.setData(indices.data(), indices.size())));
	}

	// Initialize Vertex Buffer
	m_vertexBuffer = device->createVertexBuffer(
		BufferConfig()
		.asVertexBuffer()
		.setInputLayout(inputLayout)
		.setData(vertices.data(), vertices.size()));

	// Initialize Vertex Array Object
	m_vertexArray = device->createVertexArray();
	m_vertexArray->addVertexBuffer(m_vertexBuffer);

	// Initialize Quadtree Parent Node
	m_quadTree = std::make_shared<TerrainNode>(m_context, m_indexBuffers, m_constantsModel, glm::vec3(0, 0, 0), glm::vec2(0, 0), 0);

}

Terrain::~Terrain() {
}

void Terrain::update() {

	if (m_camera->getPosition() != m_camLastPos) {
		m_camLastPos = m_camera->getPosition();
		m_quadTree->update(m_camera->getPosition());
	}
}

void Terrain::render() {

	// Bind
	m_shader->bind();
	m_textureHeight->bind(VERTEX_SHADER_BIT);
	m_textureNormal->bind(VERTEX_SHADER_BIT);
	m_textureRockDiffuse->bind();
	m_textureRockNormal->bind();
	m_constantsViewProj->bind();
	m_constantsModel->bind();

	// Update ViewProjMatrix Constants Buffer
	m_constantsViewProj->setData((void*)glm::value_ptr(m_camera->getViewProjMatrix()));

	// Draw Terrain
	m_vertexArray->bind();

	m_quadTree->render();

	m_vertexArray->unbind();

	// Unbind
	m_constantsModel->unbind();
	m_constantsViewProj->unbind();
	m_textureRockNormal->unbind();
	m_textureRockDiffuse->unbind();
	m_textureNormal->unbind(VERTEX_SHADER_BIT);
	m_textureHeight->unbind(VERTEX_SHADER_BIT);
	m_shader->unbind();
}
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
	std::vector<short> indices;
	const unsigned divs = 4;
	const float fDivs = divs;

	for (unsigned i = 0; i <= divs; ++i) {
		for (unsigned j = 0; j <= divs; ++j) {
			vertices.push_back(glm::vec2(i / fDivs, j / fDivs));

			if (i < divs && j < divs) {
				indices.push_back((i + 1) * (divs + 1) + (j + 1));
				indices.push_back((i + 1) * (divs + 1) + j);
				indices.push_back(i * (divs + 1) + j);

				indices.push_back(i * (divs + 1) + j);
				indices.push_back(i * (divs + 1) + (j + 1));
				indices.push_back((i + 1) * (divs + 1) + (j + 1));
			}
		}
	}

	// Initialize Index Buffer
	m_indexBuffer = device->createIndexBuffer(
		BufferConfig()
		.asIndexBuffer()
		.setInputLayout(InputLayoutConfig().short1())
		.setData(indices.data(), indices.size()));

	// Initialize Vertex Buffer
	m_vertexBuffer = device->createVertexBuffer(
		BufferConfig()
		.asVertexBuffer()
		.setInputLayout(inputLayout)
		.setData(vertices.data(), vertices.size()));

	// Initialize Vertex Array
	m_vertexArray = device->createVertexArray();
	m_vertexArray->addVertexBuffer(m_vertexBuffer);

	// Load Textures
	m_heightmap = device->createTexture(
		TextureConfig()
		.loadFile("res/terrain/map/heightmap.bmp")
		.setBinding(0));

	m_normalmap = device->createTexture(
		TextureConfig()
		.loadFile("res/terrain/map/normalmap.bmp")
		.setBinding(1));
	
	// Initialize Constant Buffers
	TerrainData terrainData{ glm::mat4(1), glm::vec4(scaleXZ, scaleY, scaleXZ, 0.0f) };
	terrainData.worldMatrix = glm::translate(terrainData.worldMatrix, glm::vec3(-terrainData.scale.x / 2.0f, 0.0f, -terrainData.scale.z / 2.0f));
	terrainData.worldMatrix = glm::scale(terrainData.worldMatrix, glm::vec3(terrainData.scale));

	m_constantTerrain = device->createConstantBuffer(
		BufferConfig()
		.asConstantBuffer()
		.setBinding(1)
		.setInputLayout(InputLayoutConfig().mat4().float4())
		.setData(&terrainData));

	NodeData nodeData{};
	m_constantNode = device->createConstantBuffer(
		BufferConfig()
		.asConstantBuffer()
		.setBinding(2)
		.setInputLayout(InputLayoutConfig().mat4().float2().float2().float1().int1().float2())
		.setData(&nodeData));

	// Initialize Shader
	m_shader = device->createShader(
		ShaderConfig()
		.setGLSLVertexShader("res/terrain/shader/terrain.vs.glsl")
		.setGLSLGeometryShader("res/terrain/shader/terrain.gs.glsl")
		.setGLSLPixelShader("res/terrain/shader/terrain.ps.glsl")
		.setHLSLVertexShader("res/terrain/shader/terrain.vs.hlsl", "main")
		.setHLSLPixelShader("res/terrain/shader/terrain.ps.hlsl", "main")
		.setInputLayout(inputLayout)
		.setSemanticLayout(semanticLayout));
	m_shader->bindConstants("Camera", m_constantCamera);
	m_shader->bindConstants("Terrain", m_constantTerrain);
	m_shader->bindConstants("Node", m_constantNode);
	m_shader->bindTexture("heightmap", m_heightmap);
	m_shader->bindTexture("normalmap", m_normalmap);

	// Initialize Root Node
	for (unsigned i = 0; i < rootPatches; ++i) {
		for (unsigned j = 0; j < rootPatches; ++j) {
			m_rootNodes.push_back(
				std::make_unique<TerrainNode>(
					m_context, 
					m_vertexArray, 
					m_indexBuffer, 
					m_constantNode, 
					glm::vec2(i / (float)rootPatches, j / (float)rootPatches),
					0, 
					glm::vec2(i, j)));
		}
	}
}

Terrain::~Terrain() {
}

void Terrain::update() {

	for (unsigned i = 0; i < m_rootNodes.size(); ++i) {
		m_rootNodes[i]->update(m_camera->getPosition());
	}
}

void Terrain::render() {

	// Bind Shader
	m_shader->bind();

	// Bind Textures
	m_heightmap->bind(VERTEX_SHADER_BIT);
	m_normalmap->bind(PIXEL_SHADER_BIT);

	// Bind Constants
	m_constantCamera->bind(VERTEX_SHADER_BIT | HULL_SHADER_BIT | DOMAIN_SHADER_BIT | GEOMETRY_SHADER_BIT | PIXEL_SHADER_BIT);
	m_constantTerrain->bind(VERTEX_SHADER_BIT | DOMAIN_SHADER_BIT | GEOMETRY_SHADER_BIT | PIXEL_SHADER_BIT);
	m_constantNode->bind(VERTEX_SHADER_BIT);

	// Draw Terrain
	//m_vertexArray->bind();
	//m_context->drawIndexed(m_indexBuffer, m_indexBuffer->getUnitCount(), 0);
	//m_vertexArray->unbind();
	for (unsigned i = 0; i < m_rootNodes.size(); ++i) {
		m_rootNodes[i]->render();
	}

	// Unbind All
	m_constantNode->unbind(VERTEX_SHADER_BIT);
	m_constantTerrain->unbind(VERTEX_SHADER_BIT | DOMAIN_SHADER_BIT | GEOMETRY_SHADER_BIT | PIXEL_SHADER_BIT);
	m_constantCamera->unbind(VERTEX_SHADER_BIT | HULL_SHADER_BIT | DOMAIN_SHADER_BIT | GEOMETRY_SHADER_BIT | PIXEL_SHADER_BIT);

	m_normalmap->unbind(PIXEL_SHADER_BIT);
	m_heightmap->unbind(VERTEX_SHADER_BIT);

	m_shader->unbind();
}
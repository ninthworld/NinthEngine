#include "Skydome.hpp"

Skydome::Skydome(
	const std::shared_ptr<GraphicsDevice>& device, 
	const std::shared_ptr<GraphicsContext>& context,
	const std::shared_ptr<GameCamera>& camera,
	const std::shared_ptr<ConstantBuffer>& constantCamera)
	: m_context(context)
	, m_camera(camera)
	, m_constantCamera(constantCamera)
	, m_skydomeData(SkydomeData{ glm::vec4(0.57, 0.67, 0.87, 1.0), glm::vec4(1024.0) }) {
	
	auto inputLayout = InputLayoutConfig().float3().float2();
	auto semanticLayout = SemanticLayoutConfig().position().texCoord();

	// Generate Model Data
	struct Vertex { glm::vec3 pos; glm::vec2 texCoord; };
	std::vector<Vertex> vertices;
	std::vector<short> indices;

	const int longDivs = 16;
	const int latDivs = 32;

	glm::vec2 texCoord;
	glm::vec3 pos;
	float phi, theta;
	for (unsigned i = 0; i <= longDivs; ++i) {
		texCoord.y = i / float(longDivs);
		phi = (i / float(longDivs)) * (PI / 2);
		for (unsigned j = 0; (i == 0 ? j < latDivs : j <= latDivs); ++j) {
			texCoord.x = j / float(latDivs);
			if (i == 0) {
				texCoord.x += 1 / float(2 * latDivs);
			}
			theta = (j / float(latDivs)) * (2 * PI);
			pos.y = cos(phi);
			pos.x = cos(theta) * sin(phi);
			pos.z = sin(theta) * sin(phi);

			vertices.push_back({ pos, texCoord });
		}
	}

	for (unsigned j = 0; j < latDivs; ++j) {
		indices.push_back(latDivs + j);
		indices.push_back(latDivs + (j + 1));
		indices.push_back(j);
	}

	for (unsigned i = 1; i < longDivs; ++i) {
		for (unsigned j = 0; j < latDivs; ++j) {
			indices.push_back(i * (latDivs + 1) + j + latDivs);
			indices.push_back(i * (latDivs + 1) + (j + 1) + latDivs);
			indices.push_back((i - 1) * (latDivs + 1) + j + latDivs);

			indices.push_back((i - 1) * (latDivs + 1) + (j + 1) + latDivs);
			indices.push_back((i - 1) * (latDivs + 1) + j + latDivs);
			indices.push_back(i * (latDivs + 1) + (j + 1) + latDivs);
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

	// Initialize Vertex Array Object
	m_vertexArray = device->createVertexArray();
	m_vertexArray->addVertexBuffer(m_vertexBuffer);

	// Load Textures
	m_texture = device->createTexture(
		TextureConfig()
		.loadFile("res/skydome/textures/sky4.jpg")
		.setBinding(0));
	
	m_constantSkydome = device->createConstantBuffer(
		BufferConfig()
		.asConstantBuffer()
		.setBinding(1)
		.setInputLayout(InputLayoutConfig().float4().float4())
		.setData(&m_skydomeData));

	// Load Shader
	m_shader = device->createShader(
		ShaderConfig()
		.setGLSLVertexShader("res/skydome/shaders/skydome.vs.glsl")
		.setGLSLPixelShader("res/skydome/shaders/skydome.ps.glsl")
		.setHLSLVertexShader("res/skydome/shaders/skydome.vs.hlsl", "main")
		.setHLSLPixelShader("res/skydome/shaders/skydome.ps.hlsl", "main")
		.setInputLayout(inputLayout)
		.setSemanticLayout(semanticLayout));

	// Bind Buffers to Shader
	m_shader->bindConstant("Camera", m_constantCamera);
	m_shader->bindConstant("Skydome", m_constantSkydome);
	m_shader->bindTexture("skyTexture", m_texture);
	
}

Skydome::~Skydome() {
}

void Skydome::render() {

	// Bind Shader
	m_shader->bind();

	// Bind Textures
	m_texture->bind();

	// Bind Constants
	m_constantCamera->bind();
	m_constantSkydome->bind(VERTEX_SHADER_BIT | PIXEL_SHADER_BIT);
	
	// Draw Skydome
	m_vertexArray->bind();
	m_context->drawIndexed(m_indexBuffer, m_indexBuffer->getUnitCount(), 0);
	m_vertexArray->unbind();

	// Unbind All
	m_constantSkydome->unbind(VERTEX_SHADER_BIT | PIXEL_SHADER_BIT);
	m_constantCamera->unbind();

	m_texture->unbind();

	m_shader->unbind();
}
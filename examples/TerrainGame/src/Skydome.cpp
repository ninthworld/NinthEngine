#include "Skydome.hpp"

Skydome::Skydome(
	const std::shared_ptr<GraphicsDevice>& device, 
	const std::shared_ptr<GraphicsContext>& context,
	const std::shared_ptr<GameCamera>& camera,
	const std::shared_ptr<Buffer>& constantCamera)
	: m_context(context)
	, m_camera(camera)
	, m_constantCamera(constantCamera)
	, m_skydomeData(SkydomeData{ glm::vec4(0.57f, 0.67f, 0.87f, 1.0f), glm::vec4(4000.0f) }) {
	
	auto inputLayout = LayoutConfig().float3(POSITION).float2(TEXCOORD);

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
	m_indexBuffer = device->createIndexBuffer()
		.withLayout(LayoutConfig().short1())
		.withData(indices.size(), indices.data())
		.build();

	// Initialize Vertex Buffer
	m_vertexBuffer = device->createVertexBuffer()
		.withLayout(inputLayout)
		.withData(vertices.size(), vertices.data())
		.build();

	// Initialize Vertex Array Object
	m_vertexArray = device->createVertexArray();
	m_vertexArray->addVertexBuffer(m_vertexBuffer);

	// Load Shader
	m_shader = device->createShader()
		.withLayout(inputLayout)
		.withGLSL<VERTEX_SHADER>("res/skydome/shaders/skydome.vs.glsl")
		.withGLSL<PIXEL_SHADER>("res/skydome/shaders/skydome.ps.glsl")
		.withHLSL<VERTEX_SHADER>("res/skydome/shaders/skydome.vs.hlsl", "main")
		.withHLSL<PIXEL_SHADER>("res/skydome/shaders/skydome.ps.hlsl", "main")
		.build();
	m_shader->bindConstant("Camera", m_constantCamera);

	// Initialize Constants
	m_constantSkydome = device->createConstantBuffer()
		.withLayout(LayoutConfig().float4().float4())
		.withData(&m_skydomeData)
		.build();
	m_constantSkydome->setBinding(1);
	m_shader->bindConstant("Skydome", m_constantSkydome);

	// Initialize Samplers
	m_sampler = device->createSampler()
		.withFilter(LINEAR)
		.withEdge(WRAP)
		.build();
	m_sampler->setBinding(0);

	// Load Textures
	m_texture = device->createTexture()
		.withFile("res/skydome/textures/sky4.jpg")
		.build();
	m_texture->setBinding(0);
	m_texture->setSampler(m_sampler);
	m_shader->bindTexture("skyTexture", m_texture);
	
}

Skydome::~Skydome() {
}

void Skydome::render() {

	// Bind Shader
	m_context->bind(m_shader);

	// Bind Samplers
	m_context->bind(m_sampler, PIXEL_SHADER);

	// Bind Textures
	m_context->bind(m_texture, PIXEL_SHADER);

	// Bind Constant Buffers
	m_context->bind(m_constantCamera, VERTEX_SHADER);
	m_context->bind(m_constantSkydome, VERTEX_SHADER | PIXEL_SHADER);

	// Draw Skydome
	m_context->bind(m_vertexArray);
	m_context->drawIndexed(m_indexBuffer);
}
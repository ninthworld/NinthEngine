#include "Skydome.hpp"

Skydome::Skydome(
	const std::shared_ptr<GraphicsDevice>& device,
	const std::shared_ptr<GraphicsContext>& context,
	const std::shared_ptr<ConstantBuffer>& constantCamera)
	: m_context(context)
	, m_sunTheta(0.0f)
	, m_skyStruct({
		glm::vec4(0.0f, 6.28318f, -1.0f, 0.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f) }) {
	
	initGeometry(device);
	initConstants(device);
	initShaders(device, constantCamera);
}

Skydome::~Skydome() {
}

void Skydome::initGeometry(const std::shared_ptr<GraphicsDevice>& device) {

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

	m_indexBuffer = device->createIndexBuffer()
		.withLayout(LayoutConfig().short1())
		.withData(indices.size(), indices.data())
		.build();

	m_vertexArray = device->createVertexArray();
	m_vertexArray->bind(
		device->createVertexBuffer()
			.withLayout(skydomeLayout)
			.withData(vertices.size(), vertices.data())
			.build());
}

void Skydome::initConstants(const std::shared_ptr<GraphicsDevice>& device) {
	m_constantSky = device->createConstantBuffer()
		.withLayout(skyStructLayout)
		.withData(&m_skyStruct)
		.build();
}

void Skydome::initShaders(
	const std::shared_ptr<GraphicsDevice>& device,
	const std::shared_ptr<ConstantBuffer>& constantCamera) {

	m_shader = device->createShader()
		.withLayout(skydomeLayout)
		.withGLSL<VERTEX_SHADER>("res/skydome/shaders/glsl/skydome.vs.glsl")
		.withGLSL<PIXEL_SHADER>("res/skydome/shaders/glsl/skydome.ps.glsl")
		.withHLSL<VERTEX_SHADER>("res/skydome/shaders/hlsl/skydome.vs.hlsl", "main")
		.withHLSL<PIXEL_SHADER>("res/skydome/shaders/hlsl/skydome.ps.hlsl", "main")
		.build();

	m_shader->bind(0, "Camera", constantCamera, VERTEX_SHADER);
	m_shader->bind(1, "Sky", m_constantSky, PIXEL_SHADER);
}

const static glm::vec4 dayColor = glm::vec4(0.83f, 0.93f, 0.94f, 1.0f);
const static glm::vec4 duskColor = glm::vec4(0.75f, 0.64f, 0.37f, 1.0f);
const static glm::vec4 nightColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

void Skydome::update() {

	m_sunTheta += 0.000125;
	m_sunTheta = (m_sunTheta >= PI * 2.0f ? 0.0f : m_sunTheta);

	float sunSin = sin(m_sunTheta);
	float sunCos = cos(m_sunTheta);
	m_skyStruct.sunPosition = glm::vec4(sunSin, sunCos, 0, 0);

	if (sunCos > 0.1f) {
		m_skyStruct.skyColor = dayColor;
	}
	else if (sunCos <= 0.1f && sunCos > 0.0f) {
		m_skyStruct.skyColor = glm::mix(dayColor, duskColor, (-sunCos + 0.1f) / 0.1f);
	}
	else if (sunCos <= 0.0f && sunCos > -0.075f) {
		m_skyStruct.skyColor = glm::mix(duskColor, nightColor, (-sunCos) / 0.075f);
	}
	else {
		m_skyStruct.skyColor = nightColor;
	}

	m_context->setClearColor(m_skyStruct.skyColor);

	m_context->setData(m_constantSky, &m_skyStruct);
}

void Skydome::render() {

	m_context->bind(m_shader);
	m_context->bind(m_vertexArray);
	m_context->drawIndexed(m_indexBuffer);
	m_context->unbind(m_vertexArray);
	m_context->unbind(m_shader);
}

void Skydome::onKeyboard(const Key key, const KeyState state) {
	if (key == KEY_9 && state == KS_PRESSED) m_sunTheta += 0.0125f;
}
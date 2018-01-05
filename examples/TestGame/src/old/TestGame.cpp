#include <NinthEngine\Application\GameEngine.hpp>
#include <NinthEngine\Camera\FPSGameCamera.hpp>
#include <NinthEngine\Render\Shader.hpp>
#include <NinthEngine\Render\Buffer.hpp>
#include "Skydome.hpp"
#include "Terrain.hpp"
#include "TestGame.hpp"

namespace {

struct CameraMatrix {
	glm::mat4 projMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 modelMatrix;
	glm::vec4 cameraPos;
};

} // namespace

TestGame::TestGame(const std::shared_ptr<GameEngine>& engine) 
	: m_engine(engine)
	, m_window(engine->getWindow())
	, m_device(engine->getGraphicsDevice())
	, m_context(engine->getGraphicsContext()) {

	m_camera = std::make_unique<FPSGameCamera>(glm::vec3(0, 8, 8));
	m_camera->setProjMatrix(m_window->getWidth(), m_window->getHeight());

}

TestGame::~TestGame() {

	m_camera.reset();
}

void TestGame::init() {

	m_window->setResizeCallback([this](int width, int height) {
		m_camera->setProjMatrix(width, height);
		m_context->setViewport(0, 0, width, height);
	});

	m_window->getKeyboard()->setKeyCallback([this](Key key, KeyState state) {
		if (key == KEY_ESCAPE) {
			m_window->close();
		}
		if (key == KEY_1 && state == KS_RELEASED){
			m_wireframe = !m_wireframe;

			if (!m_wireframe) {
				m_rasterizer->bind();
			}
			else {
				m_rasterizerWF->bind();
			}
		}

		m_camera->keyCallback(key, state);
	});

	m_window->getMouse()->setButtonCallback([this](MouseButton btn, MouseState state) {
		m_camera->mouseButtonCallback(m_window, btn, state);
	});

	m_window->getMouse()->setMoveCallback([this](int mx, int my) {
		m_camera->mouseMoveCallback(m_window, mx, my);
	});


	// Set Backbuffer Clear Color
	m_context->setClearColor(0.57, 0.67, 0.87, 1.0);

	// Initialize Rasterizers
	m_rasterizer = m_device->createRasterizer(
		RasterizerConfig()
		.fillSolid()
		.depthClipping()
		.multisampling()
		.cullBack()
		.frontCCW());

	m_rasterizer->bind();

	m_rasterizerWF = m_device->createRasterizer(
		RasterizerConfig()
		.fillWireframe()
		.depthClipping()
		.frontCCW());

	// Initialize Skydome
	m_skydome = std::make_unique<Skydome>(m_device, m_context, m_camera);

	// Initialize Terrain
	// m_terrain = std::make_unique<Terrain>(m_device, m_context, m_camera);

	// Test Texture
	m_textureColor = m_device->createTexture(
		TextureConfig()
		.loadFile("res/terrain/textures/Rough_rock_011/Rough_rock_011_COLOR.jpg")
		.setBinding(0));

	m_textureDisp = m_device->createTexture(
		TextureConfig()
		.loadFile("res/terrain/textures/Rough_rock_011/Rough_rock_011_DISP.jpg")
		.setBinding(1));

	m_textureNrm = m_device->createTexture(
		TextureConfig()
		.loadFile("res/terrain/textures/Rough_rock_011/Rough_rock_011_NRM.jpg")
		.setBinding(2));

	m_textureOcc = m_device->createTexture(
		TextureConfig()
		.loadFile("res/terrain/textures/Rough_rock_011/Rough_rock_011_OCC.jpg")
		.setBinding(3));

	m_textureSpec = m_device->createTexture(
		TextureConfig()
		.loadFile("res/terrain/textures/Rough_rock_011/Rough_rock_011_SPEC.jpg")
		.setBinding(4));

	CameraMatrix mvp{ glm::mat4(1), glm::mat4(1), glm::mat4(1), glm::vec4(0) };
	m_constantsCam = m_device->createConstantsBuffer(
		BufferConfig()
		.asConstantsBuffer()
		.setBinding(0)
		.setInputLayout(InputLayoutConfig().mat4().mat4().mat4().float4())
		.setData(&mvp));

	m_light.position = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	m_light.diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
	m_light.ambient = glm::vec4(0.1f, 0.1f, 0.1f, 0.0f);

	m_constantsLight = m_device->createConstantsBuffer(
		BufferConfig()
		.asConstantsBuffer()
		.setBinding(1)
		.setInputLayout(InputLayoutConfig().float4().float4().float4().float4())
		.setData(&m_light));

	auto semanticLayout = SemanticLayoutConfig().position().normal().tangent().texCoord();
	auto inputLayout = InputLayoutConfig().float3().float3().float3().float2();
	struct Vertex { glm::vec3 position, normal, tangent; glm::vec2 texCoord; };
	std::vector<Vertex> vertices;
	const unsigned divs = 16;
	for (unsigned i = 0; i <= divs; ++i) {
		for (unsigned j = 0; j <= divs; ++j) {
			float x = j / (float)divs;
			float y = i / (float)divs;
			vertices.push_back({ glm::vec3(x, 1.0f - y, y), glm::vec3(0, 1, 1), glm::vec3(1, 0, 0), glm::vec2(x, y) });
			//vertices.push_back({ glm::vec3(x, 0.0f, y), glm::vec3(0, 1, 0), glm::vec3(1, 0, 0), glm::vec2(x, y) });
		}
	}

	std::vector<short> indices;
	for (unsigned i = 0; i < divs; ++i) {
		for (unsigned j = 0; j < divs; ++j) {
			indices.push_back(i * (divs + 1) + j);
			indices.push_back((i + 1) * (divs + 1) + j);
			indices.push_back((i + 1) * (divs + 1) + (j + 1));
			indices.push_back((i + 1) * (divs + 1) + (j + 1));
			indices.push_back(i * (divs + 1) + (j + 1));
			indices.push_back(i * (divs + 1) + j);
		}
	}

	m_indexBuffer = m_device->createIndexBuffer(
		BufferConfig()
		.asIndexBuffer()
		.setInputLayout(InputLayoutConfig().short1())
		.setData(indices.data(), indices.size()));

	std::shared_ptr<VertexBuffer> vertexBuffer = m_device->createVertexBuffer(
		BufferConfig()
		.asVertexBuffer()
		.setInputLayout(inputLayout)
		.setData(vertices.data(), vertices.size()));

	m_vertexArray = m_device->createVertexArray();
	m_vertexArray->addVertexBuffer(vertexBuffer);
	
	m_shader = m_device->createShader(
		ShaderConfig()
		.setGLSLVertexShader("res/texture.vs.glsl")
		.setGLSLPixelShader("res/texture.ps.glsl")
		.setInputLayout(inputLayout)
		.setSemanticLayout(semanticLayout));
	m_shader->bindConstants("CameraMatrix", m_constantsCam);
	m_shader->bindConstants("Light", m_constantsLight);
	m_shader->bindTexture("textureColor", m_textureColor);
	m_shader->bindTexture("textureDisp", m_textureDisp);
	m_shader->bindTexture("textureNrm", m_textureNrm);

}

void TestGame::update(const double deltaTime) {

	m_camera->update(m_window, deltaTime);
	// m_terrain->update();

	/*
	m_light.position.z = sin(m_light.position.w);
	m_light.position.x = cos(m_light.position.w);
	m_light.position.w += PI * 0.0001f;
	if (m_light.position.w >= PI * 2.0f) {
		m_light.position.w = 0.0f;
	}
	*/
	m_light.position.x = m_camera->getPosition().x;
	m_light.position.y = m_camera->getPosition().y;
	m_light.position.z = m_camera->getPosition().z;
}

void TestGame::render() {

	m_context->bindBackBuffer();
	m_context->clearBackBuffer();

	m_skydome->render();

	m_shader->bind();
	m_textureColor->bind();
	m_textureDisp->bind();
	m_textureNrm->bind();

	m_constantsCam->bind();
	glm::vec4 camPos{ m_camera->getPosition().x, m_camera->getPosition().y, m_camera->getPosition().z, 0.0f };
	CameraMatrix cam{ m_camera->getProjMatrix(), m_camera->getViewMatrix(), glm::scale(glm::mat4(1), glm::vec3(8, 8, 8)), camPos };
	m_constantsCam->setData(&cam);

	m_constantsLight->bind();
	m_constantsLight->setData(&m_light);

	m_vertexArray->bind();
	m_context->drawIndexed(m_indexBuffer, m_indexBuffer->getUnitCount(), 0);
	m_vertexArray->unbind();

	m_constantsLight->unbind();
	m_constantsCam->unbind();

	m_textureNrm->unbind();
	m_textureDisp->unbind();
	m_textureColor->unbind();
	m_shader->unbind();

	// m_terrain->render();
}
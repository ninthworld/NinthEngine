#include "ShadowGame.hpp"

ShadowGame::ShadowGame(const std::shared_ptr<GameEngine>& engine)
	: m_engine(engine)
	, m_window(engine->getWindow())
	, m_device(engine->getGraphicsDevice())
	, m_context(engine->getGraphicsContext())
	, m_skyStruct({ glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) }) {

	m_camera = std::make_shared<FPSGameCamera>(glm::vec3(4, 4, 4), glm::vec3(0), 8.0f);
	m_camera->setProjMatrix(m_window->getWidth(), m_window->getHeight());

	m_cameraDebug = std::make_shared<FPSGameCamera>(glm::vec3(8), glm::vec3(PI / 6, -PI / 4, 0), 8.0f);
	m_cameraDebug->setProjMatrix(m_window->getWidth(), m_window->getHeight());
}

ShadowGame::~ShadowGame() {
}

void ShadowGame::init() {

	m_context->setClearColor({ 1.0f, 1.0f, 1.0f, 1.0f });

	initRasterizers();
	initSamplers();
	initGeometry();
	initConstants();
	initRenderTargets();
	initShaders();
}

void ShadowGame::initRasterizers() {

	m_rasterizerWireframe = m_device->createRasterizer()
		.withFill(WIREFRAME)
		.withDepthClipping()
		.withMultisampling()
		.withFrontCCW()
		.build();

	m_rasterizerDefault = m_device->createRasterizer()
		.withFill()
		.withDepthClipping()
		.withMultisampling()
		.withCull()
		.withFrontCCW()
		.build();
	m_context->bind(m_rasterizerDefault);
}

void ShadowGame::initSamplers() {
	m_sampler = m_device->createSampler()
		.withEdge(WRAP)
		.withFilter(LINEAR)
		.build();
}

void ShadowGame::initGeometry() {

	struct Vertex { glm::vec3 position, color, normal; };

	// Frustum Geometry
	float aspectRatio = float(m_window->getWidth()) / float(m_window->getHeight());
	float fov = m_camera->getFOV();
	float near = m_camera->getZNear();
	float far = 64.0f; // m_camera->getZFar();

	float nearHalfWidth = near * tan(fov);
	float nearHalfHeight = nearHalfWidth / aspectRatio;
	float farHalfWidth = far * tan(fov);
	float farHalfHeight = farHalfWidth / aspectRatio;

	m_frustumPoints[0] = glm::vec3(-nearHalfWidth, nearHalfHeight, near);	// Near Left Top
	m_frustumPoints[1] = glm::vec3(-nearHalfWidth, -nearHalfHeight, near);	// Near Left Bottom
	m_frustumPoints[2] = glm::vec3(nearHalfWidth, nearHalfHeight, near);	// Near Right Top
	m_frustumPoints[3] = glm::vec3(nearHalfWidth, -nearHalfHeight, near);	// Near Right Bottom
	m_frustumPoints[4] = glm::vec3(-farHalfWidth, farHalfHeight, far);		// Far Left Top
	m_frustumPoints[5] = glm::vec3(-farHalfWidth, -farHalfHeight, far);		// Far Left Bottom
	m_frustumPoints[6] = glm::vec3(farHalfWidth, farHalfHeight, far);		// Far Right Top
	m_frustumPoints[7] = glm::vec3(farHalfWidth, -farHalfHeight, far);		// Far Right Bottom
	m_frustumPoints[8] = glm::vec3(0, 0, (far - near) / 2.0f + near);		// Center

	std::vector<Vertex> verticesFrustum{
		// Top
		{ m_frustumPoints[2], glm::vec3(), glm::vec3() },
		{ m_frustumPoints[6], glm::vec3(), glm::vec3() },
		{ m_frustumPoints[4], glm::vec3(), glm::vec3() },
		{ m_frustumPoints[0], glm::vec3(), glm::vec3() },
		// Bottom
		{ m_frustumPoints[5], glm::vec3(), glm::vec3() },
		{ m_frustumPoints[7], glm::vec3(), glm::vec3() },
		{ m_frustumPoints[3], glm::vec3(), glm::vec3() },
		{ m_frustumPoints[1], glm::vec3(), glm::vec3() },
		// Right
		{ m_frustumPoints[3], glm::vec3(), glm::vec3() },
		{ m_frustumPoints[7], glm::vec3(), glm::vec3() },
		{ m_frustumPoints[6], glm::vec3(), glm::vec3() },
		{ m_frustumPoints[2], glm::vec3(), glm::vec3() },
		// Left
		{ m_frustumPoints[4], glm::vec3(), glm::vec3() },
		{ m_frustumPoints[5], glm::vec3(), glm::vec3() },
		{ m_frustumPoints[1], glm::vec3(), glm::vec3() },
		{ m_frustumPoints[0], glm::vec3(), glm::vec3() },
		// Front
		{ m_frustumPoints[3], glm::vec3(), glm::vec3() },
		{ m_frustumPoints[2], glm::vec3(), glm::vec3() },
		{ m_frustumPoints[0], glm::vec3(), glm::vec3() },
		{ m_frustumPoints[1], glm::vec3(), glm::vec3() },
		// Front
		{ m_frustumPoints[4], glm::vec3(), glm::vec3() },
		{ m_frustumPoints[6], glm::vec3(), glm::vec3() },
		{ m_frustumPoints[7], glm::vec3(), glm::vec3() },
		{ m_frustumPoints[5], glm::vec3(), glm::vec3() },
	};

	m_vertexArrayFrustum = m_device->createVertexArray();
	m_vertexArrayFrustum->bind(m_device->createVertexBuffer()
		.withLayout(sceneLayout)
		.withData(verticesFrustum.size(), verticesFrustum.data())
		.build());

	// Plane Geometry
	glm::vec3 colorPlane(0.1f, 0.4f, 0.9f);
	std::vector<Vertex> verticesPlane{
		{ glm::vec3(-0.5f, 0.0f, -0.5f), colorPlane, glm::vec3(0, 1, 0) },
		{ glm::vec3( 0.5f, 0.0f,  0.5f), colorPlane, glm::vec3(0, 1, 0) },
		{ glm::vec3( 0.5f, 0.0f, -0.5f), colorPlane, glm::vec3(0, 1, 0) },
		{ glm::vec3(-0.5f, 0.0f, -0.5f), colorPlane, glm::vec3(0, 1, 0) },
		{ glm::vec3(-0.5f, 0.0f,  0.5f), colorPlane, glm::vec3(0, 1, 0) },
		{ glm::vec3( 0.5f, 0.0f,  0.5f), colorPlane, glm::vec3(0, 1, 0) },
	};

	m_vertexArrayPlane = m_device->createVertexArray();
	m_vertexArrayPlane->bind(m_device->createVertexBuffer()
		.withLayout(sceneLayout)
		.withData(verticesPlane.size(), verticesPlane.data())
		.build());
	
	// Cube Geometry
	std::vector<Vertex> verticesCube{
		// Top
		{ glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0) },
		{ glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0) },
		{ glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0) },
		{ glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0, 1, 1), glm::vec3(0, 1, 0) },
		// Bottom
		{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1, 1, 0), glm::vec3(0, -1, 0) },
		{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1, 0, 1), glm::vec3(0, -1, 0) },
		{ glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(1, 0, 0), glm::vec3(0, -1, 0) },
		{ glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(1, 1, 1), glm::vec3(0, -1, 0) },
		// Right
		{ glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0) },
		{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1, 0, 1), glm::vec3(1, 0, 0) },
		{ glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0, 0, 1), glm::vec3(1, 0, 0) },
		{ glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0, 1, 0), glm::vec3(1, 0, 0) },
		// Left
		{ glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0, 0, 0), glm::vec3(-1, 0, 0) },
		{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1, 1, 0), glm::vec3(-1, 0, 0) },
		{ glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(1, 1, 1), glm::vec3(-1, 0, 0) },
		{ glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0, 1, 1), glm::vec3(-1, 0, 0) },
		// Front
		{ glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(1, 0, 0), glm::vec3(0, 0, 1) },
		{ glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1) },
		{ glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0, 1, 1), glm::vec3(0, 0, 1) },
		{ glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(1, 1, 1), glm::vec3(0, 0, 1) },
		// Front
		{ glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0, 0, 0), glm::vec3(0, 0, -1) },
		{ glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0, 0, 1), glm::vec3(0, 0, -1) },
		{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1, 0, 1), glm::vec3(0, 0, -1) },
		{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1, 1, 0), glm::vec3(0, 0, -1) }
	};

	m_vertexArrayCube = m_device->createVertexArray();
	m_vertexArrayCube->bind(m_device->createVertexBuffer()
		.withLayout(sceneLayout)
		.withData(verticesCube.size(), verticesCube.data())
		.build());

	std::vector<short> indicesCube{
		0, 1, 2, 2, 3, 0, // +Y
		4, 5, 6, 6, 7, 4, // -Y
		8, 9, 10, 10, 11, 8, // +X
		12, 13, 14, 14, 15, 12, // -X
		16, 17, 18, 18, 19, 16, // +Z
		20, 21, 22, 22, 23, 20 // -Z
	};

	m_indexBufferCube = m_device->createIndexBuffer()
		.withLayout(LayoutConfig().short1())
		.withData(indicesCube.size(), indicesCube.data())
		.build();

	// Quad Geometry
	std::vector<glm::vec2> verticesQuad{
		glm::vec2(-1, -1), glm::vec2(1, -1), glm::vec2(1, 1),
		glm::vec2(1, 1), glm::vec2(-1, 1), glm::vec2(-1, -1) };

	m_vertexArrayQuad = m_device->createVertexArray();
	m_vertexArrayQuad->bind(m_device->createVertexBuffer()
		.withLayout(fxLayout)
		.withData(verticesQuad.size(), verticesQuad.data())
		.build());
}

void ShadowGame::initConstants() {

	m_constantCamera = m_device->createConstantBuffer()
		.withLayout(cameraStructLayout)
		.withData(&m_camera->getStruct())
		.build();

	m_constantCameraExt = m_device->createConstantBuffer()
		.withLayout(cameraExtStructLayout)
		.withData(&m_camera->getExtStruct())
		.build();

	ModelStruct modelStruct{ glm::mat4(1) };
	m_constantModel = m_device->createConstantBuffer()
		.withLayout(modelStructLayout)
		.withData(&modelStruct)
		.build();

	m_constantSky = m_device->createConstantBuffer()
		.withLayout(skyStructLayout)
		.withData(&m_skyStruct)
		.build();

	m_constantShadowMap = m_device->createConstantBuffer()
		.withLayout(cameraStructLayout)
		.withData(&m_camera->getStruct())
		.build();
}

void ShadowGame::initRenderTargets() {
	m_renderTargetScene = m_device->createRenderTarget()
		.withRenderTarget(m_window->getWidth(), m_window->getHeight())	// Color
		.withRenderTarget(m_window->getWidth(), m_window->getHeight())	// Normals
		.withRenderTarget(m_window->getWidth(), m_window->getHeight(), FORMAT_DEPTH_24_STENCIL_8)	// Depth
		.build();
	m_renderTargetScene->getTexture(0)->setSampler(m_sampler);
	m_renderTargetScene->getTexture(1)->setSampler(m_sampler);
	m_renderTargetScene->getDepthTexture()->setSampler(m_sampler);

	unsigned shadowMapSize = 2048;
	m_renderTargetShadowMap = m_device->createRenderTarget()
		.withRenderTarget(shadowMapSize, shadowMapSize)
		.withRenderTarget(shadowMapSize, shadowMapSize, FORMAT_DEPTH_24_STENCIL_8)
		.build();
	m_renderTargetShadowMap->getDepthTexture()->setSampler(m_sampler);
}

void ShadowGame::initShaders() {

	m_shaderScene = m_device->createShader()
		.withLayout(sceneLayout)
		.withGLSL<VERTEX_SHADER>("res/ShadowGame/scene/shaders/glsl/scene.vs.glsl")
		.withGLSL<PIXEL_SHADER>("res/ShadowGame/scene/shaders/glsl/scene.ps.glsl")
		.withHLSL<VERTEX_SHADER>("res/ShadowGame/scene/shaders/hlsl/scene.vs.hlsl", "main")
		.withHLSL<PIXEL_SHADER>("res/ShadowGame/scene/shaders/hlsl/scene.ps.hlsl", "main")
		.build();
	m_shaderScene->bind(0, "Camera", m_constantCamera, VERTEX_SHADER);
	m_shaderScene->bind(1, "Model", m_constantModel, VERTEX_SHADER);

	m_shaderShadowMap = m_device->createShader()
		.withLayout(sceneLayout)
		.withGLSL<VERTEX_SHADER>("res/ShadowGame/scene/shaders/glsl/shadowmap/sceneShadow.vs.glsl")
		.withGLSL<PIXEL_SHADER>("res/ShadowGame/scene/shaders/glsl/shadowmap/sceneShadow.ps.glsl")
		.withHLSL<VERTEX_SHADER>("res/ShadowGame/scene/shaders/hlsl/shadowmap/sceneShadow.vs.hlsl", "main")
		.withHLSL<PIXEL_SHADER>("res/ShadowGame/scene/shaders/hlsl/shadowmap/sceneShadow.ps.hlsl", "main")
		.build();
	m_shaderShadowMap->bind(0, "ShadowMap", m_constantShadowMap, VERTEX_SHADER);
	m_shaderShadowMap->bind(1, "Model", m_constantModel, VERTEX_SHADER);

	m_shaderFX = m_device->createShader()
		.withLayout(fxLayout)
		.withGLSL<VERTEX_SHADER>("res/ShadowGame/fx/glsl/fx.vs.glsl")
		.withGLSL<PIXEL_SHADER>("res/ShadowGame/fx/glsl/lighting/lighting.ps.glsl")
		.withHLSL<VERTEX_SHADER>("res/ShadowGame/fx/hlsl/fx.vs.hlsl", "main")
		.withHLSL<PIXEL_SHADER>("res/ShadowGame/fx/hlsl/lighting/lighting.ps.hlsl", "main")
		.build();
	m_shaderFX->bind(0, "Sky", m_constantSky, PIXEL_SHADER);
	m_shaderFX->bind(1, "Camera", m_constantCamera, PIXEL_SHADER);
	m_shaderFX->bind(2, "CameraExt", m_constantCameraExt, PIXEL_SHADER);
	m_shaderFX->bind(3, "ShadowMap", m_constantShadowMap, PIXEL_SHADER);
	m_shaderFX->bind(0, "texSampler", m_sampler, PIXEL_SHADER);
	m_shaderFX->bind(0, "colorTexture", m_renderTargetScene->getTexture(0), PIXEL_SHADER);
	m_shaderFX->bind(1, "normalTexture", m_renderTargetScene->getTexture(1), PIXEL_SHADER);
	m_shaderFX->bind(2, "depthTexture", m_renderTargetScene->getDepthTexture(), PIXEL_SHADER);
	m_shaderFX->bind(3, "shadowDepthTexture", m_renderTargetShadowMap->getDepthTexture(), PIXEL_SHADER);
}

float angle = 0.0f;
void ShadowGame::update(const double deltaTime) {

	// Update Camera
	if (m_debug) m_cameraDebug->update(deltaTime);
	else m_camera->update(deltaTime);

	angle += 0.001f;
	//m_skyStruct.sunPosition = glm::vec4(cos(angle), 1.0f, sin(angle), 1.0f);
	m_context->setData(m_constantSky, &m_skyStruct);

	// Update Camera Constant Buffer
	if (m_debug) {
		m_context->setData(m_constantCamera, &m_cameraDebug->getStruct());
		m_context->setData(m_constantCameraExt, &m_cameraDebug->getExtStruct());
	}
	else {
		m_context->setData(m_constantCamera, &m_camera->getStruct());
		m_context->setData(m_constantCameraExt, &m_camera->getExtStruct());
	}
}

const static float OFFSET = 10;
const static glm::vec4 UP = glm::vec4(0, 1, 0, 0);
const static glm::vec4 FORWARD = glm::vec4(0, 0, -1, 0);
const static float SHADOW_DISTANCE = 100;

void ShadowGame::render() {

	glm::vec3 lightDir = glm::normalize(m_skyStruct.sunPosition);
	glm::vec3 lightRotation(
		acos(lightDir.y),
		atan2(lightDir.x, lightDir.z),
		0.0f);	
	
	glm::mat4 camRotateMatrix = glm::mat4(1);
	camRotateMatrix *= glm::rotate(glm::mat4(1), float(PI) - m_camera->getRotation().y, glm::vec3(0, 1, 0));
	camRotateMatrix *= glm::rotate(glm::mat4(1), m_camera->getRotation().x, glm::vec3(1, 0, 0));

	glm::mat4 lightRotateMatrix(1.0f);
	lightRotateMatrix *= glm::rotate(glm::mat4(1), lightRotation.y, glm::vec3(0, 1, 0));
	lightRotateMatrix *= glm::rotate(glm::mat4(1), lightRotation.x, glm::vec3(1, 0, 0));

	glm::vec3 points[8];
	for (unsigned i = 0; i < 8; ++i) {
		points[i] = lightRotateMatrix * camRotateMatrix * glm::vec4(m_frustumPoints[i], 1.0f);
	}

	glm::vec3 center = glm::vec3(camRotateMatrix * glm::vec4(m_frustumPoints[8], 1.0f)) + m_camera->getPosition();
	glm::vec3 min(FLT_MAX);
	glm::vec3 max(FLT_MIN);
	for (unsigned i = 0; i < 8; ++i) {
		for (unsigned j = 0; j < 3; ++j) {
			if (points[i][j] < min[j]) min[j] = points[i][j];
			if (points[i][j] > max[j]) max[j] = points[i][j];
		}
	}

	glm::mat4 depthProjectionMatrix = glm::ortho<float>(min.x, max.x, min.y, max.y, min.z, max.z);
	glm::mat4 depthViewMatrix = glm::mat4(1);
	depthViewMatrix *= glm::translate(glm::mat4(1), -center);
	//depthViewMatrix *= glm::lookAt(lightDir, glm::vec3(0), glm::vec3(0, 1, 0));
	//depthViewMatrix *= glm::rotate(glm::mat4(1), lightRotation.x, glm::vec3(1, 0, 0));
	//depthViewMatrix *= glm::rotate(glm::mat4(1), lightRotation.y, glm::vec3(0, 1, 0));

	CameraStruct cameraStruct;
	cameraStruct.camViewProj = depthProjectionMatrix * depthViewMatrix;
	m_context->setData(m_constantShadowMap, &cameraStruct);

	ModelStruct modelStruct{ glm::mat4(1) };
	
	for (unsigned i = 0; i < 2; ++i) {
		if (i == 0) {
			m_context->bind(m_renderTargetShadowMap);
			m_context->clear(m_renderTargetShadowMap);
			m_context->bind(m_shaderShadowMap);
		}
		else {
			m_context->bind(m_renderTargetScene);
			m_context->clear(m_renderTargetScene);
			m_context->bind(m_shaderScene);
		}

		modelStruct.modelTransform = glm::scale(glm::mat4(1), glm::vec3(100));
		m_context->setData(m_constantModel, &modelStruct);
		m_context->bind(m_vertexArrayPlane);
		m_context->draw(6);

		m_context->bind(m_vertexArrayCube);
		for (unsigned j = 0; j < 4; ++j) {
			modelStruct.modelTransform = glm::translate(glm::mat4(1), glm::vec3(16 * floor(j/2), 1, 16 * (j%2)));
			m_context->setData(m_constantModel, &modelStruct);
			m_context->drawIndexed(m_indexBufferCube);
		}
	}

	m_context->bind(m_rasterizerWireframe);

	// Draw Ortho
	modelStruct.modelTransform = glm::translate(glm::mat4(1), center);
	modelStruct.modelTransform *= lightRotateMatrix;
	modelStruct.modelTransform *= glm::scale(glm::mat4(1), max - min);
	m_context->setData(m_constantModel, &modelStruct);
	m_context->drawIndexed(m_indexBufferCube);

	// Draw Frustum
	modelStruct.modelTransform = glm::translate(glm::mat4(1), m_camera->getPosition());
	modelStruct.modelTransform *= camRotateMatrix;
	m_context->setData(m_constantModel, &modelStruct);
	m_context->bind(m_vertexArrayFrustum);
	m_context->drawIndexed(m_indexBufferCube);

	m_context->bind(m_rasterizerDefault);

	m_context->bindBackBuffer();
	m_context->clearBackBuffer();

	m_context->bind(m_shaderFX);

	m_context->bind(m_vertexArrayQuad);
	m_context->draw(6);

	m_context->unbind(m_shaderFX);
}

void ShadowGame::onResize(const int width, const int height) {
	m_camera->setProjMatrix(width, height);
}

void ShadowGame::onKeyboard(const Key key, const KeyState state) {
	if (key == KEY_ESCAPE) m_window->close();
	if (key == KEY_0 && state == KS_RELEASED) m_debug = !m_debug;
	if (m_debug) m_cameraDebug->onKeyboard(key, state);
	else m_camera->onKeyboard(key, state);
}

void ShadowGame::onMouseButton(const MouseButton button, const MouseState state) {
	if (m_debug) m_cameraDebug->onMouseButton(m_window, button, state);
	else m_camera->onMouseButton(m_window, button, state);
}

void ShadowGame::onMouseMove(const int x, const int y) {
	if (m_debug) m_cameraDebug->onMouseMove(m_window, x, y);
	else m_camera->onMouseMove(m_window, x, y);
}
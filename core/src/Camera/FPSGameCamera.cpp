#include <algorithm>
#include "..\..\include\NinthEngine\Application\GameWindow.hpp"
#include "..\..\include\NinthEngine\Input\Keyboard.hpp"
#include "..\..\include\NinthEngine\Input\Mouse.hpp"
#include "..\..\include\NinthEngine\Camera\FPSGameCamera.hpp"

namespace NinthEngine {

FPSGameCamera::FPSGameCamera(const glm::vec3 position, const glm::vec3 rotation, const FPSGameCameraSettings settings)
	: m_position(position)
	, m_rotation(rotation)
	, m_settings(settings)
	, m_mouseDelta(glm::vec2()) {
}

FPSGameCamera::~FPSGameCamera() {
}

void FPSGameCamera::update(const std::shared_ptr<GameWindow>& window, const double deltaTime) {
	
	m_rotation.x += m_mouseDelta.y; // *deltaTime;
	m_rotation.y += m_mouseDelta.x; // *deltaTime;
	m_mouseDelta = glm::vec2();
	
	m_rotation.x = std::max<float>(-PI / 2.001, fmin(PI / 2.001, m_rotation.x));
	m_rotation.y += (m_rotation.y < 0 ? 2.0 * PI : (m_rotation.y > 2.0 * PI ? -2.0 * PI : 0));
	
	auto movement = glm::vec3();
	double sinXRot = sin(m_rotation.x);
	double cosXRot = cos(m_rotation.x);
	double sinYRot = sin(m_rotation.y);
	double cosYRot = cos(m_rotation.y);
	double pitchLimitFactor = cosXRot;

	if (window->getKeyboard()->getKey(KEY_W) == KS_PRESSED) {
		movement.x += sinYRot * pitchLimitFactor;
		movement.z -= cosYRot * pitchLimitFactor;
	}

	if (window->getKeyboard()->getKey(KEY_S) == KS_PRESSED) {
		movement.x -= sinYRot * pitchLimitFactor;
		movement.z += cosYRot * pitchLimitFactor;
	}

	if (window->getKeyboard()->getKey(KEY_A) == KS_PRESSED) {
		movement.x += -cosYRot;
		movement.z += -sinYRot;
	}

	if (window->getKeyboard()->getKey(KEY_D) == KS_PRESSED) {
		movement.x += cosYRot;
		movement.z += sinYRot;
	}

	if (window->getKeyboard()->getKey(KEY_SPACE) == KS_PRESSED) {
		movement.y++;
	}

	if (window->getKeyboard()->getKey(KEY_SHIFT) == KS_PRESSED) {
		movement.y--;
	}

	movement = (glm::length(movement) > 0 ? glm::normalize(movement) : movement);
	double fpsFactor = getFPSSettings().moveSpeedFactor * deltaTime;
	movement *= fpsFactor;

	m_position += movement;

	m_data.position = glm::vec4(m_position, 0.0f);

	setViewMatrix();
}

void FPSGameCamera::setProjMatrix(const int width, const int height) {
	float w = std::max<float>(width, 1);
	float h = std::max<float>(height, 1);
	float wDivH = w / h;

	m_projMatrix = glm::perspective(getSettings().FOV, wDivH, getSettings().zNear, getSettings().zFar);

	float tanFov = tan((getSettings().FOV / 2.0f) * PI / 180.0);

	for (unsigned i = 0; i < 6; ++i) {

		float inv = (i % 2 ? -1 : 1);
		int half = floor(i / 2);

		m_frustumPlanes[i] = glm::normalize(glm::vec4(
			m_projMatrix[3][0] + inv * m_projMatrix[half][0] * (i < 2 ? tanFov * wDivH : 1),
			m_projMatrix[3][1] + inv * m_projMatrix[half][1] * (i >= 2 && i < 4 ? tanFov : 1),
			m_projMatrix[3][2] + inv * m_projMatrix[half][2],
			m_projMatrix[3][3] + inv * m_projMatrix[half][3]));

		m_data.frustumPlanes[i] = m_frustumPlanes[i];
	}
	
	setViewMatrix();
}

void FPSGameCamera::setViewMatrix() {
	m_viewMatrix = glm::rotate(glm::mat4(1), m_rotation.x, glm::vec3(1, 0, 0));
	m_viewMatrix = glm::rotate(m_viewMatrix, m_rotation.y, glm::vec3(0, 1, 0));
	m_viewMatrix = glm::rotate(m_viewMatrix, m_rotation.z, glm::vec3(0, 0, 1));
	m_viewMatrix = glm::translate(m_viewMatrix, -m_position);

	m_data.viewMatrix = m_viewMatrix;
	
	setViewProjMatrix();
}

void FPSGameCamera::setViewProjMatrix() {
	m_viewProjMatrix = m_projMatrix * m_viewMatrix;
	m_data.viewProjMatrix = m_viewProjMatrix;

}

void FPSGameCamera::mouseMoveCallback(const std::shared_ptr<GameWindow>& window, int x, int y) {

	if (window->getMouse()->getButton(MB_RIGHT_BTN) == MS_PRESSED) {
		
		m_mouseDelta = glm::vec2(
			(x - (window->getWidth() / 2)) * getFPSSettings().yawSensitivity,
			(y - (window->getHeight() / 2)) * getFPSSettings().pitchSensitivity
		);
	}
}

void FPSGameCamera::mouseButtonCallback(const std::shared_ptr<GameWindow>& window, MouseButton button, MouseState state) {

	if (window->getMouse()->getButton(MB_RIGHT_BTN) == MS_PRESSED) {
		window->setMouseVisible(false);
		window->getMouse()->setMouseCentered(true);
	}
	else {
		window->setMouseVisible(true);
		window->getMouse()->setMouseCentered(false);
	}
}

void FPSGameCamera::keyCallback(Key key, KeyState state) {
	
}

} // namespace NinthEngine
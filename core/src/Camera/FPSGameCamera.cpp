#include "..\..\include\NinthEngine\Camera\FPSGameCamera.hpp"

namespace NinthEngine {

FPSGameCamera::FPSGameCamera(
	const glm::vec3 position,
	const glm::vec3 rotation,
	const float moveFactor,
	const float yawFactor,
	const float pitchFactor,
	const float fov,
	const float zNear,
	const float zFar,
	const int width,
	const int height)
	: m_moveFactor(moveFactor)
	, m_yawFactor(yawFactor)
	, m_pitchFactor(pitchFactor)
	, m_mouseDelta(glm::vec2(0))
	, GameCamera(
		position,
		rotation,
		fov,
		zNear,
		zFar,
		width,
		height) {
}

FPSGameCamera::~FPSGameCamera() {
}

void FPSGameCamera::update(const double deltaTime) {
	
	glm::vec3 rotation = getRotation();

	rotation.x += m_mouseDelta.y;
	rotation.y += m_mouseDelta.x;
	m_mouseDelta = glm::vec2(0);
	
	rotation.x = std::max<float>(-PI / 2.001, fmin(PI / 2.001, rotation.x));
	rotation.y += (rotation.y < 0 ? 2.0 * PI : (rotation.y > 2.0 * PI ? -2.0 * PI : 0));
	
	double sinXRot = sin(rotation.x);
	double cosXRot = cos(rotation.x);
	double sinYRot = sin(rotation.y);
	double cosYRot = cos(rotation.y);
	double pitchLimitFactor = cosXRot;

	auto movement = glm::vec3(0);
	if (m_keyState[KEY_W] == KS_PRESSED) {
		movement.x += sinYRot * pitchLimitFactor;
		movement.z -= cosYRot * pitchLimitFactor;
	}

	if (m_keyState[KEY_S] == KS_PRESSED) {
		movement.x -= sinYRot * pitchLimitFactor;
		movement.z += cosYRot * pitchLimitFactor;
	}

	if (m_keyState[KEY_A] == KS_PRESSED) {
		movement.x += -cosYRot;
		movement.z += -sinYRot;
	}

	if (m_keyState[KEY_D] == KS_PRESSED) {
		movement.x += cosYRot;
		movement.z += sinYRot;
	}

	if (m_keyState[KEY_SPACE] == KS_PRESSED) {
		movement.y++;
	}

	if (m_keyState[KEY_SHIFT] == KS_PRESSED) {
		movement.y--;
	}

	movement = (glm::length(movement) > 0 ? glm::normalize(movement) : movement);
	movement *= m_moveFactor * deltaTime;

	setPosition(getPosition() + movement);
	setRotation(rotation);

	setViewMatrix();
}

void FPSGameCamera::onKeyboard(const Key key, const KeyState state) {
	m_keyState[key] = state;
}

void FPSGameCamera::onMouseButton(const std::shared_ptr<GameWindow>& window, const const MouseButton button, const MouseState state) {
	
	m_mouseState[button] = state;
	if (m_mouseState[MB_RIGHT_BTN] == MS_PRESSED) {
		window->setMouseVisible(false);
		window->getMouse()->setMouseCentered(true);
	}
	else {
		window->setMouseVisible(true);
		window->getMouse()->setMouseCentered(false);
	}
}

void FPSGameCamera::onMouseMove(const std::shared_ptr<GameWindow>& window, const int x, const int y) {

	if (m_mouseState[MB_RIGHT_BTN] == MS_PRESSED) {
		m_mouseDelta = glm::vec2(
			(x - (window->getWidth() / 2.0f)) * m_yawFactor,
			(y - (window->getHeight() / 2.0f)) * m_pitchFactor);
	}
}

} // namespace NinthEngine
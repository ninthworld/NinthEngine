#include <algorithm>
#include "..\..\include\NinthEngine\Application\GameWindow.hpp"
#include "..\..\include\NinthEngine\Input\Keyboard.hpp"
#include "..\..\include\NinthEngine\Input\Mouse.hpp"
#include "..\..\include\NinthEngine\Camera\FPSGameCamera.hpp"

namespace NinthEngine {

FPSGameCamera::FPSGameCamera(const glm::vec3 position, const glm::vec3 rotation,const FPSGameCameraSettings settings)
	: position(position), rotation(rotation), settings(settings), mouseDelta(glm::vec2()) {
}

void FPSGameCamera::init(const int width, const int height) {
	setProjMatrix(width, height);
}

void FPSGameCamera::update(const std::shared_ptr<GameWindow>& window, const double deltaTime) {
	
	rotation.x += mouseDelta.y * deltaTime;
	rotation.y += mouseDelta.x * deltaTime;
	mouseDelta = glm::vec2();
	
	rotation.x = fmax(-PI / 2.0, fmin(PI / 2.0, rotation.x));
	rotation.y += (rotation.y < 0 ? 2.0 * PI : (rotation.y > 2.0 * PI ? -2.0 * PI : 0));
	
	glm::vec3 movement = glm::vec3();
	double sinXRot = sin(rotation.x);
	double cosXRot = cos(rotation.x);
	double sinYRot = sin(rotation.y);
	double cosYRot = cos(rotation.y);
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

	position += movement;

	setViewMatrix();
}

void FPSGameCamera::setProjMatrix(const int width, const int height) {
	float w = std::max(width, 1);
	float h = std::max(height, 1);

	projMatrix = glm::perspective(getSettings().FOV, w / h, getSettings().zNear, getSettings().zFar);

	setViewMatrix();
}

void FPSGameCamera::setViewMatrix() {
	viewMatrix = glm::rotate(glm::mat4(1), rotation.x, glm::vec3(1, 0, 0));
	viewMatrix = glm::rotate(viewMatrix, rotation.y, glm::vec3(0, 1, 0));
	viewMatrix = glm::rotate(viewMatrix, rotation.z, glm::vec3(0, 0, 1));
	viewMatrix = glm::translate(viewMatrix, -position);

	setViewProjMatrix();
}

void FPSGameCamera::setViewProjMatrix() {
	viewProjMatrix = projMatrix * viewMatrix;
}

void FPSGameCamera::mouseMoveCallback(const std::shared_ptr<GameWindow>& window, int mouseX, int mouseY) {

	if (window->getMouse()->getButton(MB_RIGHT_BTN) == MS_PRESSED) {
		
		mouseDelta = glm::vec2(
			(mouseX - (window->getWidth() / 2)) * getFPSSettings().yawSensitivity,
			(mouseY - (window->getHeight() / 2)) * getFPSSettings().pitchSensitivity
		);
	}
}

void FPSGameCamera::mouseButtonCallback(const std::shared_ptr<GameWindow>& window, MouseButton button, MouseState action) {

	if (window->getMouse()->getButton(MB_RIGHT_BTN) == MS_PRESSED) {
		window->setMouseVisible(false);
		window->getMouse()->setMouseCentered(true);
	}
	else {
		window->setMouseVisible(true);
		window->getMouse()->setMouseCentered(false);
	}
}

void FPSGameCamera::keyCallback(Key key, KeyState action) {
	
}

} // namespace NinthEngine
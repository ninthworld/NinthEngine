#include "..\include\NinthEngine\Camera.hpp"
#include "..\include\NinthEngine\Window.hpp"
#include "..\include\NinthEngine\GameUtils.hpp"

namespace NinthEngine {

Camera::Camera(const glm::vec3 position, const glm::vec3 rotation, const float fov, const float zNear, const float zFar, const double pitchSensitivity, const double yawSensitivity, const double moveSpeedFactor) :
	position(position),
	rotation(rotation),
	fov(fov),
	zNear(zNear),
	zFar(zFar),
	pitchSensitivity(pitchSensitivity),
	yawSensitivity(yawSensitivity),
	moveSpeedFactor(moveSpeedFactor),
	lockMouse(false) {

	for (int i = 0; i < sizeof(keyDown) / sizeof(*keyDown); i++) keyDown[i] = false;
}

void Camera::init(Window *window) {
	setProjMatrix(window->getWidth(), window->getHeight());
	setViewMatrix();
}

void Camera::update(const float interval) {

	glm::vec3 movement(0);
	double sinXRot = sin(rotation.x);
	double cosXRot = cos(rotation.x);
	double sinYRot = sin(rotation.y);
	double cosYRot = cos(rotation.y);
	double pitchLimitFactor = cosXRot;

	if (keyDown[GLFW_KEY_W]) {
		movement.x += sinYRot * pitchLimitFactor;
		movement.z -= cosYRot * pitchLimitFactor;
	}

	if (keyDown[GLFW_KEY_S]) {
		movement.x -= sinYRot * pitchLimitFactor;
		movement.z += cosYRot * pitchLimitFactor;
	}

	if (keyDown[GLFW_KEY_A]) {
		movement.x += -cosYRot;
		movement.z += -sinYRot;
	}

	if (keyDown[GLFW_KEY_D]) {
		movement.x += cosYRot;
		movement.z += sinYRot;
	}

	if (keyDown[GLFW_KEY_SPACE]) {
		movement.y++;
	}

	if (keyDown[GLFW_KEY_LEFT_SHIFT]) {
		movement.y--;
	}

	movement = (glm::length(movement) > 0 ? glm::normalize(movement) : movement);
	double fpsFactor = moveSpeedFactor * interval;
	movement *= fpsFactor;
	position += movement;

	setViewMatrix();
}

void Camera::input(Window *window) {
	if (lockMouse) {
		double horizontal = (window->getMouseX() - window->getWindowMidX()) * yawSensitivity;
		double vertical = (window->getMouseY() - window->getWindowMidY()) * pitchSensitivity;

		rotation.x += vertical;
		rotation.y += horizontal;

		rotation.x = fmax(-PI / 2.0, fmin(PI / 2.0, rotation.x));
		rotation.y += (rotation.y < 0 ? 2.0 * PI : (rotation.y > 2.0 * PI ? -2.0 * PI : 0));

		window->setMouseCentered();
		window->setMouseHidden(true);
	}
	else {
		window->setMouseHidden(false);
	}

	lockMouse = window->isMousePressed(GLFW_MOUSE_BUTTON_2);
	if (lockMouse) window->setMouseCentered();

	keyDown[GLFW_KEY_W] = window->isKeyPressed(GLFW_KEY_W);
	keyDown[GLFW_KEY_A] = window->isKeyPressed(GLFW_KEY_A);
	keyDown[GLFW_KEY_S] = window->isKeyPressed(GLFW_KEY_S);
	keyDown[GLFW_KEY_D] = window->isKeyPressed(GLFW_KEY_D);
	keyDown[GLFW_KEY_SPACE] = window->isKeyPressed(GLFW_KEY_SPACE);
	keyDown[GLFW_KEY_LEFT_SHIFT] = window->isKeyPressed(GLFW_KEY_LEFT_SHIFT);
}

void Camera::setProjMatrix(const int width, const int height) {
	projMatrix = glm::perspective(fov, (GLfloat)width / (GLfloat)height, zNear, zFar);
}

void Camera::setViewMatrix() {
	viewMatrix = glm::rotate(glm::mat4(), rotation.x, glm::vec3(1, 0, 0));
	viewMatrix = glm::rotate(viewMatrix, rotation.y, glm::vec3(0, 1, 0));
	viewMatrix = glm::rotate(viewMatrix, rotation.z, glm::vec3(0, 0, 1));
	viewMatrix = glm::translate(viewMatrix, -position);

	setViewProjMatrix();
}

void Camera::setViewProjMatrix() {
	viewProjMatrix = projMatrix * viewMatrix;
}

} // namespace NinthEngine
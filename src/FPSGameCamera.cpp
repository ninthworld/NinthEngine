#include "..\include\NinthEngine\FPSGameCamera.hpp"
#include "..\include\NinthEngine\GameWindow.hpp"

#define PI 3.14159

namespace NinthEngine {

FPSGameCamera::FPSGameCamera(GameCameraSettings settings, const glm::vec3 position, const glm::vec3 rotation, FPSGameCameraSettings fpsSettings)
	: GameCamera(settings, position, rotation)
	, keyStates(std::vector<InputState>(512, RELEASED))
	, mouseStates(std::vector<InputState>(3, RELEASED))
	, mouseDelta(glm::vec2(0))
	, fpsSettings(fpsSettings) {
}

FPSGameCamera::~FPSGameCamera() {
}

void FPSGameCamera::init(const int width, const int height) {
	setProjMatrix(width, height);
}

void FPSGameCamera::mouseMoveCallback(GameWindow *window, double mouseX, double mouseY) {

	if (mouseStates[MB_RIGHT]) {
		mouseDelta = glm::vec2(
			(mouseX - (window->getWidth() / 2.0)) * getFPSSettings().yawSensitivity,
			(mouseY - (window->getHeight() / 2.0)) * getFPSSettings().pitchSensitivity
		);
	}
}

void FPSGameCamera::mouseButtonCallback(GameWindow *window, int button, InputState action) {
	if (button > mouseStates.size()) return;

	mouseStates[button] = action;

	if (mouseStates[MB_RIGHT] == PRESSED) {
		window->setMouseVisible(false);
		window->setMouseCentered(true);
	}
	else {
		window->setMouseVisible(true);
		window->setMouseCentered(false);
	}
}

void FPSGameCamera::keyCallback(int keyCode, InputState action) {
	if (keyCode > keyStates.size()) return;

	keyStates[keyCode] = action;
}


void FPSGameCamera::update(const double deltaTime) {

	glm::vec3 rotation = getRotation();

	rotation.x += mouseDelta.y * deltaTime;
	rotation.y += mouseDelta.x * deltaTime;
	mouseDelta = glm::vec2(0);

	rotation.x = fmax(-PI / 2.0, fmin(PI / 2.0, rotation.x));
	rotation.y += (rotation.y < 0 ? 2.0 * PI : (rotation.y > 2.0 * PI ? -2.0 * PI : 0));

	setRotation(rotation);

	glm::vec3 movement(0);
	double sinXRot = sin(getRotation().x);
	double cosXRot = cos(getRotation().x);
	double sinYRot = sin(getRotation().y);
	double cosYRot = cos(getRotation().y);
	double pitchLimitFactor = cosXRot;

	if (keyStates[VK_W]) {
		movement.x += sinYRot * pitchLimitFactor;
		movement.z -= cosYRot * pitchLimitFactor;
	}

	if (keyStates[VK_S]) {
		movement.x -= sinYRot * pitchLimitFactor;
		movement.z += cosYRot * pitchLimitFactor;
	}

	if (keyStates[VK_A]) {
		movement.x += -cosYRot;
		movement.z += -sinYRot;
	}

	if (keyStates[VK_D]) {
		movement.x += cosYRot;
		movement.z += sinYRot;
	}

	if (keyStates[VK_SPACE]) {
		movement.y++;
	}

	if (keyStates[VK_LSHIFT]) {
		movement.y--;
	}
			
	movement = (glm::length(movement) > 0 ? glm::normalize(movement) : movement);
	double fpsFactor = getFPSSettings().moveSpeedFactor * deltaTime;
	movement *= fpsFactor;

	setPosition(getPosition() + movement);

	setViewMatrix();
}

} // namespace NinthEngine
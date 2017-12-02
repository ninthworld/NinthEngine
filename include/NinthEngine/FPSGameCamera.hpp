#pragma once

#include "..\NinthEngine\GameInput.hpp"
#include "..\NinthEngine\GameCamera.hpp"
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <vector>

namespace NinthEngine {

struct FPSGameCameraSettings {
	float moveSpeedFactor = 12.0f;
	float yawSensitivity = 6.0f;
	float pitchSensitivity = 6.0f;
};

class GameWindow;

class FPSGameCamera : public GameCamera {
public:
	FPSGameCamera(const GameCameraSettings = GameCameraSettings(), const glm::vec3 position = glm::vec3(0), const glm::vec3 rotation = glm::vec3(0), FPSGameCameraSettings settings = FPSGameCameraSettings());
	~FPSGameCamera();

	void init(const int width, const int height);
	void update(const double deltaTime);

	void mouseMoveCallback(GameWindow *window, double mouseX, double mouseY);
	void mouseButtonCallback(GameWindow *window, int button, InputState action);
	void keyCallback(int keyCode, InputState action);

	FPSGameCameraSettings getFPSSettings() const;

	void setFPSSettings(FPSGameCameraSettings);

protected:
private:
	FPSGameCameraSettings fpsSettings;

	std::vector<InputState> keyStates;
	std::vector<InputState> mouseStates;
	glm::vec2 mouseDelta;

};

inline FPSGameCameraSettings FPSGameCamera::getFPSSettings() const {
	return fpsSettings;
}

inline void FPSGameCamera::setFPSSettings(FPSGameCameraSettings _settings) {
	fpsSettings = _settings;
}

} // namespace NinthEngine
#pragma once

#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <vector>
#include "GameCamera.hpp"

namespace NinthEngine {

struct FPSGameCameraSettings : public GameCameraSettings {
	float moveSpeedFactor = 12.0f;
	float yawSensitivity = 6.0f;
	float pitchSensitivity = 6.0f;
};

enum Key;
enum KeyState;
enum MouseButton;
enum MouseState;
class GameWindow;

class FPSGameCamera : public GameCamera {
public:
	FPSGameCamera(const glm::vec3 position = glm::vec3(0), const glm::vec3 rotation = glm::vec3(0), const FPSGameCameraSettings = FPSGameCameraSettings());
	~FPSGameCamera() {};

	void init(const int width, const int height);
	void update(const std::shared_ptr<GameWindow>& window, const double deltaTime);

	GameCameraSettings getSettings() const { return settings; };
	FPSGameCameraSettings getFPSSettings() const { return settings; };
	glm::vec3 getPosition() const { return position; };
	glm::vec3 getRotation() const { return rotation; };
	glm::mat4 getProjMatrix() const { return projMatrix; };
	glm::mat4 getViewMatrix() const { return viewMatrix; };
	glm::mat4 getViewProjMatrix() const { return viewProjMatrix; };

	void setSettings(GameCameraSettings _settings) { settings.FOV = _settings.FOV; settings.zFar = _settings.zFar; settings.zNear = _settings.zNear; };
	void setFPSSettings(FPSGameCameraSettings _settings) { settings = _settings; };
	void setPosition(const glm::vec3 _position) { position = _position; };
	void setRotation(const glm::vec3 _rotation) { rotation = _rotation; };
	void setProjMatrix(const int width, const int height);
	void setViewMatrix();
	void setViewProjMatrix();

	void mouseMoveCallback(const std::shared_ptr<GameWindow>&, double, double);
	void mouseButtonCallback(const std::shared_ptr<GameWindow>&, MouseButton, MouseState);
	void keyCallback(Key, KeyState);

protected:
private:
	FPSGameCameraSettings settings;

	glm::vec3 position;
	glm::vec3 rotation;

	glm::mat4 projMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 viewProjMatrix;

	glm::vec2 mouseLast;
	glm::vec2 mouseDelta;

};

} // namespace NinthEngine
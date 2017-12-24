#pragma once

#include <vector>
#include "GameCamera.hpp"

namespace NinthEngine {

struct FPSGameCameraSettings : public GameCameraSettings {
	float moveSpeedFactor = 16.f;
	float yawSensitivity = 0.001f;
	float pitchSensitivity = 0.001f;
};

enum Key;
enum KeyState;
enum MouseButton;
enum MouseState;
class GameWindow;

class FPSGameCamera : public GameCamera {
public:
	FPSGameCamera(const glm::vec3 position = glm::vec3(), const glm::vec3 rotation = glm::vec3(), const FPSGameCameraSettings = FPSGameCameraSettings());
	~FPSGameCamera();

	void init(const int width, const int height) override;
	void update(const std::shared_ptr<GameWindow>& window, const double deltaTime) override;

	const GameCameraSettings getSettings() const override { return m_settings; };
	const FPSGameCameraSettings getFPSSettings() const { return m_settings; };
	const glm::vec3 getPosition() const override { return m_position; };
	const glm::vec3 getRotation() const override { return m_rotation; };
	const glm::mat4 getProjMatrix() const override { return m_projMatrix; };
	const glm::mat4 getViewMatrix() const override { return m_viewMatrix; };
	const glm::mat4 getViewProjMatrix() const override { return m_viewProjMatrix; };

	void setSettings(GameCameraSettings settings) override { m_settings.FOV = settings.FOV; m_settings.zFar = settings.zFar; m_settings.zNear = settings.zNear; };
	void setFPSSettings(FPSGameCameraSettings settings) { m_settings = settings; };
	void setPosition(const glm::vec3 position) override { m_position = position; };
	void setRotation(const glm::vec3 rotation) override { m_rotation = rotation; };
	void setProjMatrix(const int width, const int height) override;
	void setViewMatrix() override;
	void setViewProjMatrix() override;

	void mouseMoveCallback(const std::shared_ptr<GameWindow>& window, int x, int y);
	void mouseButtonCallback(const std::shared_ptr<GameWindow>& window, MouseButton button, MouseState state);
	void keyCallback(Key key, KeyState state);

protected:
private:
	FPSGameCameraSettings m_settings;

	glm::vec3 m_position;
	glm::vec3 m_rotation;

	glm::mat4 m_projMatrix;
	glm::mat4 m_viewMatrix;
	glm::mat4 m_viewProjMatrix;

	glm::vec2 m_mouseDelta;

};

} // namespace NinthEngine
#pragma once

#include <memory>
#include "..\Utils\MathUtils.hpp"

namespace NinthEngine {

struct Camera {
	glm::mat4 viewMatrix;
	glm::mat4 viewProjMatrix;
	glm::vec4 position;
};

struct GameCameraSettings {
	float FOV = 45.f;
	float zNear = 0.1f;
	float zFar = 4096.f;
};

class GameWindow;

class GameCamera {
public:
	GameCamera() = default;
	GameCamera(const GameCamera&) = delete;
	GameCamera& operator=(const GameCamera&) = delete;

	virtual ~GameCamera() = default;

	virtual void update(const std::shared_ptr<GameWindow>& window, const double deltaTime) = 0;

	virtual const GameCameraSettings getSettings() const = 0;
	virtual const glm::vec3 getPosition() const = 0;
	virtual const glm::vec3 getRotation() const = 0;
	virtual const glm::mat4 getProjMatrix() const = 0;
	virtual const glm::mat4 getViewMatrix() const = 0;
	virtual const glm::mat4 getViewProjMatrix() const = 0;

	virtual void setSettings(GameCameraSettings) = 0;
	virtual void setPosition(const glm::vec3) = 0;
	virtual void setRotation(const glm::vec3) = 0;
	virtual void setProjMatrix(const int width, const int height) = 0;
	virtual void setViewMatrix() = 0;
	virtual void setViewProjMatrix() = 0;

	virtual Camera data() = 0;

};

} // namespace NinthEngine
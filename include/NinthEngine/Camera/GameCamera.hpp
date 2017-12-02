#pragma once

#include <glm\glm.hpp>
#include <glm\ext.hpp>

namespace NinthEngine {

struct GameCameraSettings {
	float FOV = 45.f;
	float zNear = 0.1f;
	float zFar = 1000.f;
};

class GameCamera {
public:
	virtual void init(const int width, const int height) = 0;
	virtual void update(const double deltaTime) = 0;

	virtual GameCameraSettings getSettings() const = 0;
	virtual glm::vec3 getPosition() const = 0;
	virtual glm::vec3 getRotation() const = 0;
	virtual glm::mat4 getProjMatrix() const = 0;
	virtual glm::mat4 getViewMatrix() const = 0;
	virtual glm::mat4 getViewProjMatrix() const = 0;

	virtual void setSettings(GameCameraSettings) = 0;
	virtual void setPosition(const glm::vec3) = 0;
	virtual void setRotation(const glm::vec3) = 0;
	virtual void setProjMatrix(const int width, const int height) = 0;
	virtual void setViewMatrix() = 0;
	virtual void setViewProjMatrix() = 0;

};

} // namespace NinthEngine
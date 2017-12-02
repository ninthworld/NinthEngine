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
	GameCamera(GameCameraSettings settings = GameCameraSettings(), const glm::vec3 position = glm::vec3(0), const glm::vec3 rotation = glm::vec3(0))
		: settings(settings), position(position), rotation(rotation) {};
	virtual ~GameCamera() {};

	virtual void init(const int width, const int height) = 0;
	virtual void update(const double deltaTime) = 0;

	virtual GameCameraSettings getSettings() const;
	virtual glm::vec3 getPosition() const;
	virtual glm::vec3 getRotation() const;
	virtual glm::mat4 getProjMatrix() const;
	virtual glm::mat4 getViewMatrix() const;
	virtual glm::mat4 getViewProjMatrix() const;

	virtual void setSettings(GameCameraSettings);
	virtual void setPosition(const glm::vec3);
	virtual void setRotation(const glm::vec3);
	virtual void setProjMatrix(const int width, const int height);
	virtual void setViewMatrix();
	virtual void setViewProjMatrix();

protected:
private:
	GameCameraSettings settings;

	glm::vec3 position;
	glm::vec3 rotation;

	glm::mat4 projMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 viewProjMatrix;
};

inline GameCameraSettings GameCamera::getSettings() const {
	return settings;
}

inline glm::vec3 GameCamera::getPosition() const {
	return position;
}

inline glm::vec3 GameCamera::getRotation() const {
	return rotation;
}

inline glm::mat4 GameCamera::getProjMatrix() const {
	return projMatrix;
}

inline glm::mat4 GameCamera::getViewMatrix() const {
	return viewMatrix;
}

inline glm::mat4 GameCamera::getViewProjMatrix() const {
	return viewProjMatrix;
}

inline void GameCamera::setSettings(GameCameraSettings _settings) {
	settings = _settings;
}

inline void GameCamera::setPosition(const glm::vec3 _position) {
	position = _position;
}

inline void GameCamera::setRotation(const glm::vec3 _rotation) {
	rotation = _rotation;
}

inline void GameCamera::setProjMatrix(const int width, const int height) {
	projMatrix = glm::perspective(getSettings().FOV, static_cast<float>(width) / static_cast<float>(height), getSettings().zNear, getSettings().zFar);

	setViewMatrix();
}

inline void GameCamera::setViewMatrix() {
	viewMatrix = glm::rotate(glm::mat4(1), rotation.x, glm::vec3(1, 0, 0));
	viewMatrix = glm::rotate(viewMatrix, rotation.y, glm::vec3(0, 1, 0));
	viewMatrix = glm::rotate(viewMatrix, rotation.z, glm::vec3(0, 0, 1));
	viewMatrix = glm::translate(viewMatrix, -position);

	setViewProjMatrix();
}

inline void GameCamera::setViewProjMatrix() {
	viewProjMatrix = projMatrix * viewMatrix;
}

} // namespace NinthEngine
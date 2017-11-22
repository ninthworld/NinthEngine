#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <glm\gtx\extend.hpp>

namespace NinthEngine {

class Window;

class Camera {
public:
	Camera(const glm::vec3 position, const glm::vec3 rotation, const float fov = 45.0f, const float zNear = 0.1f, const float zFar = 1000.0f, const double pitchSensitivity = 0.002, const double yawSensitivity = 0.002, const double moveSpeedFactor = 1.0);
	void init(Window *window);
	void update(const float interval);
	void input(Window *window);
	glm::mat4 getViewProjMatrix() { return viewProjMatrix; }

protected:
	void setProjMatrix(const int width, const int height);
	void setViewMatrix();
	void setViewProjMatrix();

private:
	float fov;
	float zNear;
	float zFar;
	double pitchSensitivity;
	double yawSensitivity;
	double moveSpeedFactor;
	bool keyDown[256];
	bool lockMouse;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::mat4 projMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 viewProjMatrix;
};

} // namespace NinthEngine
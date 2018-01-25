#pragma once

#include <memory>
#include <array>
#include "..\Utils\MathUtils.hpp"
#include "..\Render\Config\LayoutConfig.hpp"

namespace NinthEngine {

const static LayoutConfig cameraStructLayout = 
	LayoutConfig().float4x4().float4();
struct CameraStruct {
	glm::mat4 camViewProj;
	glm::vec4 camPosition;
};
/*
* GLSL
	layout(std140) uniform Camera {
		mat4 camViewProj;
		vec4 camPosition;
	};

* HLSL
	cbuffer Camera : register(b?) {
		float4x4 camViewProj;
		float4 camPosition;
	};
*/

const static LayoutConfig cameraExtStructLayout =
	LayoutConfig().float4x4().float4x4().float4x4().float4x4();
struct CameraExtStruct {
	glm::mat4 camView;
	glm::mat4 camProj;
	glm::mat4 camInvView;
	glm::mat4 camInvProj;
};
/*
* GLSL
	layout(std140) uniform CameraExt {
		mat4 camView;
		mat4 camProj;
		mat4 camInvView;
		mat4 camInvProj;
	};

* HLSL
	cbuffer CameraExt : register(b?) {
		float4x4 camView;
		float4x4 camProj;
		float4x4 camInvView;
		float4x4 camInvProj;
	};
*/

class GameCamera {
public:
	GameCamera(
		const glm::vec3 position = glm::vec3(0), 
		const glm::vec3 rotation = glm::vec3(0),
		const float fov = 45.0f,
		const float zNear = 0.1f,
		const float zFar = 1000.0f,
		const int width = 1,
		const int height = 1);
	virtual ~GameCamera();

	virtual void update(const double deltaTime);
	
	virtual void onResize(const int width, const int height);

	const glm::vec3 getPosition() const { return m_position; };
	const glm::vec3 getRotation() const { return m_rotation; };
	const float getFOV() const { return m_fov; };
	const float getZNear() const { return m_zNear; };
	const float getZFar() const { return m_zFar; };

	const glm::mat4 getProjMatrix() const { return m_projMatrix; };
	const glm::mat4 getViewMatrix() const { return m_viewMatrix; };
	const glm::mat4 getViewProjMatrix() const { return m_viewProjMatrix; };

	CameraStruct getStruct() { return m_struct; };
	CameraExtStruct getExtStruct() { return m_extStruct; };

	void setPosition(const glm::vec3 position) { m_position = position; m_struct.camPosition = glm::vec4(position, 1.0f); };
	void setRotation(const glm::vec3 rotation) { m_rotation = rotation; };
	void setFOV(const float fov) { m_fov = fov; };
	void setZNear(const float zNear) { m_zNear = zNear; };
	void setZFar(const float zFar) { m_zFar = zFar; };

	void setProjMatrix(const glm::mat4 projMatrix) { m_projMatrix = projMatrix; m_extStruct.camProj = projMatrix; };
	void setViewMatrix(const glm::mat4 viewMatrix) { m_viewMatrix = viewMatrix; m_extStruct.camView = viewMatrix; };
	void setViewProjMatrix(const glm::mat4 viewProjMatrix) { m_viewProjMatrix = viewProjMatrix; m_struct.camViewProj = viewProjMatrix; };

	virtual void setProjMatrix(const int width, const int height);
	virtual void setViewMatrix();
	virtual void setViewProjMatrix();

	const bool inFrustum(const AABB box) const;

private:
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	float m_fov;
	float m_zNear;
	float m_zFar;

	glm::mat4 m_projMatrix;
	glm::mat4 m_viewMatrix;
	glm::mat4 m_viewProjMatrix;

	CameraStruct m_struct;
	CameraExtStruct m_extStruct;

	std::array<glm::vec4, 6> m_frustumPlanes;

};

} // namespace NinthEngine
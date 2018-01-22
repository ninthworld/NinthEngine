#include "..\..\include\NinthEngine\Camera\GameCamera.hpp"

namespace NinthEngine {

GameCamera::GameCamera(
	const glm::vec3 position,
	const glm::vec3 rotation,
	const float fov,
	const float zNear,
	const float zFar,
	const int width,
	const int height)
	: m_position(position) 
	, m_rotation(rotation) 
	, m_fov(fov)
	, m_zNear(zNear)
	, m_zFar(zFar) {
	setProjMatrix(width, height);
}

GameCamera::~GameCamera() {
}

void GameCamera::update(const double deltaTime) {
}

void GameCamera::onResize(const int width, const int height) {
	setProjMatrix(width, height);
}

void GameCamera::setProjMatrix(const int width, const int height) {
	m_projMatrix = glm::perspective(
		m_fov, 
		std::max((float)width, 1.0f) / std::max((float)height, 1.0f), 
		m_zNear,
		m_zFar);

	m_extStruct.camProj = m_projMatrix;
	m_extStruct.camInvProj = glm::mat4(
		1 / m_projMatrix[0][0], 0, 0, 0,
		0, 1 / m_projMatrix[1][1], 0, 0,
		0, 0, 0, 1 / m_projMatrix[3][2],
		0, 0, 1 / m_projMatrix[2][3], -m_projMatrix[2][2] / (m_projMatrix[2][3] * m_projMatrix[3][2]));

	setViewMatrix();
}

void GameCamera::setViewMatrix() {
	m_viewMatrix = glm::rotate(glm::mat4(1), m_rotation.x, glm::vec3(1, 0, 0));
	m_viewMatrix = glm::rotate(m_viewMatrix, m_rotation.y, glm::vec3(0, 1, 0));
	m_viewMatrix = glm::rotate(m_viewMatrix, m_rotation.z, glm::vec3(0, 0, 1));
	m_viewMatrix = glm::translate(m_viewMatrix, -m_position);
	m_extStruct.camView = m_viewMatrix;

	setViewProjMatrix();
}

void GameCamera::setViewProjMatrix() {
	m_viewProjMatrix = m_projMatrix * m_viewMatrix;
	m_struct.camViewProj = m_viewProjMatrix;

	glm::mat4 tViewProjMatrix = glm::transpose(m_viewProjMatrix);
	for (unsigned i = 0; i < 3; ++i) {
		m_frustumPlanes[i * 2] = glm::normalize(tViewProjMatrix[i] + tViewProjMatrix[3]);
		m_frustumPlanes[i * 2 + 1] = glm::normalize(-tViewProjMatrix[i] + tViewProjMatrix[3]);
	}
}

const bool GameCamera::inFrustum(const AABB box) const {
	// Code from http://www.iquilezles.org/www/articles/frustumcorrect/frustumcorrect.htm
	// inigo quilez 1994-2017
	for (int i = 0; i<6; i++) {
		int out = 0;
		out += ((glm::dot(m_frustumPlanes[i], glm::vec4(box.min.x, box.min.y, box.min.z, 1.0f)) < 0.0f) ? 1 : 0);
		out += ((glm::dot(m_frustumPlanes[i], glm::vec4(box.max.x, box.min.y, box.min.z, 1.0f)) < 0.0f) ? 1 : 0);
		out += ((glm::dot(m_frustumPlanes[i], glm::vec4(box.min.x, box.max.y, box.min.z, 1.0f)) < 0.0f) ? 1 : 0);
		out += ((glm::dot(m_frustumPlanes[i], glm::vec4(box.max.x, box.max.y, box.min.z, 1.0f)) < 0.0f) ? 1 : 0);
		out += ((glm::dot(m_frustumPlanes[i], glm::vec4(box.min.x, box.min.y, box.max.z, 1.0f)) < 0.0f) ? 1 : 0);
		out += ((glm::dot(m_frustumPlanes[i], glm::vec4(box.max.x, box.min.y, box.max.z, 1.0f)) < 0.0f) ? 1 : 0);
		out += ((glm::dot(m_frustumPlanes[i], glm::vec4(box.min.x, box.max.y, box.max.z, 1.0f)) < 0.0f) ? 1 : 0);
		out += ((glm::dot(m_frustumPlanes[i], glm::vec4(box.max.x, box.max.y, box.max.z, 1.0f)) < 0.0f) ? 1 : 0);
		if (out == 8) return false;
	}
	return true;
}

} // namespace NinthEngine
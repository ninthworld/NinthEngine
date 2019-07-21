#include "NinthEngine/NEPCH.h"
#include "NinthEngine/Scene/Camera/Camera.h"

namespace NinthEngine {

Camera::Camera(Node* parent) 
	: SceneNode(parent)
	, m_viewMatrix(glm::mat4(1.0f))
	, m_projMatrix(glm::mat4(1.0f))
	, m_viewProjMatrix(glm::mat4(1.0f)) {}

Camera::~Camera() {}

void Camera::updateLocal() {
	Node::updateLocal();
	updateCamera();
	m_viewMatrix = glm::translate(glm::toMat4(getLocalRotation()), getWorldPosition() * -1.0f);
	m_viewProjMatrix = m_projMatrix * m_viewMatrix;
}

void Camera::setViewMatrix(const glm::mat4& viewMatrix) { 
	m_viewMatrix = viewMatrix; 
	onLocalModified(); 
}

void Camera::setProjMatrix(const glm::mat4& projMatrix) { 
	m_projMatrix = projMatrix; 
	onLocalModified(); 
}

}
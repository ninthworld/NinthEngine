#pragma once

#include "NinthEngine/Scene/Node.h"

namespace NinthEngine {

struct CameraData {
	glm::mat4 projMatrix, viewMatrix, viewProjMatrix;
	CameraData() : projMatrix(glm::mat4(1.0f)), viewMatrix(glm::mat4(1.0f)), viewProjMatrix(glm::mat4(1.0f)) {};
};

class NE_API Camera : public Node {
protected:
	Camera(Node* parent)
		: Node(parent), m_zNear(0.1f), m_zFar(1000.0f)
		, m_frustum(glm::mat4(1.0f)), m_data(CameraData()) {};

public:
	virtual ~Camera() {};

	inline virtual void updateLocal() override {
		Node::updateLocal();
		updateCamera();
		m_data.viewMatrix = glm::translate(glm::toMat4(getLocalRotation()), getWorldPosition() * -1.0f);
		m_data.viewProjMatrix = m_data.projMatrix * m_data.viewMatrix;
	};

	inline virtual const float getZNear() const { return m_zNear; };
	inline virtual const float getZFar() const { return m_zFar; };
	inline virtual glm::mat4 getViewMatrix() { return m_data.viewMatrix; };
	inline virtual glm::mat4 getProjMatrix() { return m_data.projMatrix; };
	inline virtual glm::mat4 getFrustum() { return m_frustum; };

	inline virtual void setZNear(const float zNear) { m_zNear = zNear; };
	inline virtual void setZFar(const float zFar) { m_zFar = zFar; };
	inline virtual void setViewMatrix(const glm::mat4& viewMatrix) { m_data.viewMatrix = viewMatrix; };
	inline virtual void setProjMatrix(const glm::mat4& projMatrix) { m_data.projMatrix = projMatrix; };
	inline virtual void setFrustum(const glm::mat4& frustum) { m_frustum = frustum; };
	
	inline virtual CameraData getData() { return m_data; };
	
protected:
	virtual void updateCamera() = 0;

protected:
	CameraData m_data;

private:
	float m_zNear, m_zFar;
	glm::mat4 m_frustum;
};

}
#pragma once

#include "NinthEngine/Scene/SceneNode.h"

namespace NinthEngine {

struct CameraData {
	glm::mat4 projMatrix, viewMatrix;
};

class NE_API Camera : public SceneNode {
public:
	Camera(const std::shared_ptr<SceneNode>& parent, const std::shared_ptr<Scene>& scene)
		: SceneNode(parent, scene)
		, m_zNear(0.1f), m_zFar(1000.0f) {
		m_scene->addCamera(std::dynamic_pointer_cast<Camera>(shared_from_this()));
	};
	virtual ~Camera() {};

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

	inline virtual void destroy() override {
		SceneNode::destroy();
		m_scene->removeCamera(std::dynamic_pointer_cast<Camera>(shared_from_this()));
	};

protected:
	inline virtual void updateNode() override {
		SceneNode::updateNode();
		m_data.viewMatrix = getWorldTransform();
		updateCamera();
	};

	virtual void updateCamera() = 0;

private:
	float m_zNear, m_zFar;
	glm::mat4 m_frustum;
	CameraData m_data;
};

}
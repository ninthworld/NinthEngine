#pragma once

#include "NinthEngine/Scene/SceneNode.h"

namespace NinthEngine {

enum class NE_API CameraType {
	Perspective,
	Orthographic
};

class NE_API Camera : public SceneNode {
protected:
	Camera(Node* parent);
public:
	virtual ~Camera();

	virtual void updateLocal() override;

	inline virtual glm::mat4 getViewMatrix() { return m_viewMatrix; }
	inline virtual glm::mat4 getProjMatrix() { return m_projMatrix; }
	inline virtual glm::mat4 getViewProjMatrix() { return m_viewProjMatrix; }

	void setViewMatrix(const glm::mat4& viewMatrix);
	void setProjMatrix(const glm::mat4& projMatrix);

	virtual CameraType getCameraType() = 0;

protected:
	virtual void updateCamera() = 0;

protected:
	glm::mat4 m_viewMatrix;
	glm::mat4 m_projMatrix;
	glm::mat4 m_viewProjMatrix;
};

}
#pragma once

#include "Camera.h"

namespace NinthEngine {

struct NE_API PerspectiveCameraProps {
	float aspectRatio;
	float fieldOfView;
	float zNear;
	float zFar;
	PerspectiveCameraProps(
		const float aspectRatio = 1.0f, const float fieldOfView = 45.0f, 
		const float zNear = 0.1f, const float zFar = 1000.0f)
		: aspectRatio(aspectRatio), fieldOfView(fieldOfView)
		, zNear(zNear), zFar(zFar) {}
};

class NE_API PerspectiveCamera : public Camera {
	friend class SceneNode;
protected:
	PerspectiveCamera(Node* parent, const PerspectiveCameraProps& props);
public:
	~PerspectiveCamera();

	inline const float getZNear() const { return m_zNear; }
	inline const float getZFar() const { return m_zFar; }
	inline const float getFOV() const { return m_fov; };
	inline const float getAspectRatio() const { return m_aspect; };

	void setZNear(const float zNear);
	void setZFar(const float zFar);
	void setFieldOfView(const float fov);
	void setAspectRatio(const float aspect);

	virtual CameraType getCameraType() override { return CameraType::Perspective; }

protected:
	void updateCamera() override;

private:
	float m_zNear, m_zFar;
	float m_fov, m_aspect;
};

}
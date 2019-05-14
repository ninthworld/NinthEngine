#pragma once

#include "Camera.h"

namespace NinthEngine {

class NE_API PerspectiveCamera : public Camera {
public:
	PerspectiveCamera(const std::shared_ptr<SceneNode>& parent, const std::shared_ptr<Scene>& scene);
	~PerspectiveCamera();

	inline const float getFOV() const { return m_fov; };
	inline const float getAspectRatio() const { return m_aspect; };

	inline void setFOV(const float fov) { m_fov = fov; };
	inline void setAspectRatio(const float aspect) { m_aspect = aspect; };

protected:
	void updateCamera() override;

private:
	float m_fov, m_aspect;
};

}
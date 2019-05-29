#pragma once

#include "Camera.h"

namespace NinthEngine {

class NE_API PerspectiveCamera : public Camera {
protected:
	PerspectiveCamera(Node* parent, const float aspectRatio = 1.0f);

public:
	~PerspectiveCamera();

	inline const float getFOV() const { return m_fov; };
	inline const float getAspectRatio() const { return m_aspect; };

	inline void setFOV(const float fov) { m_fov = fov; };
	inline void setAspectRatio(const float aspect) { m_aspect = aspect; };

	static PerspectiveCamera* create(Node* parent, const float aspectRatio);

protected:
	void updateCamera() override;

private:
	float m_fov, m_aspect;
};

}
#include "pch.h"
#include "PerspectiveCamera.h"

namespace NinthEngine {

PerspectiveCamera::PerspectiveCamera(const std::shared_ptr<SceneNode>& parent, const std::shared_ptr<Scene>& scene)
	: Camera(parent, scene), m_fov(45.0f), m_aspect(1.0f) {
}

PerspectiveCamera::~PerspectiveCamera() {
}

void PerspectiveCamera::updateCamera() {
	setProjMatrix(glm::perspective(m_fov, m_aspect, getZNear(), getZFar()));
}

}
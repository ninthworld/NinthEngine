#include "NinthEngine/NEPCH.h"
#include "NinthEngine/Scene/Camera/PerspectiveCamera.h"

namespace NinthEngine {

PerspectiveCamera::PerspectiveCamera(Node* parent, const PerspectiveCameraProps& props)
	: Camera(parent), m_aspect(props.aspectRatio), m_fov(props.fieldOfView)
	, m_zNear(props.zNear), m_zFar(props.zFar) {}

PerspectiveCamera::~PerspectiveCamera() {}

void PerspectiveCamera::updateCamera() {
	setProjMatrix(glm::perspective(m_fov, m_aspect, getZNear(), getZFar()));
}

void PerspectiveCamera::setZNear(const float zNear) {
	m_zNear = zNear;
	onLocalModified();
}

void PerspectiveCamera::setZFar(const float zFar) {
	m_zFar = zFar;
	onLocalModified();
}

void PerspectiveCamera::setFieldOfView(const float fov) {
	m_fov = fov;
	onLocalModified();
}

void PerspectiveCamera::setAspectRatio(const float aspect) {
	m_aspect = aspect;
	onLocalModified();
}

}
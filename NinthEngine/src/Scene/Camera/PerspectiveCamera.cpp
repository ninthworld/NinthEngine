#include "pch.h"
#include "PerspectiveCamera.h"

namespace NinthEngine {

PerspectiveCamera* PerspectiveCamera::create(Node* parent, const float aspectRatio) {
	auto node = new PerspectiveCamera(parent, aspectRatio);
	if (parent != nullptr) parent->addChild(node);
	return node;
};

PerspectiveCamera::PerspectiveCamera(Node* parent, const float aspectRatio)
	: Camera(parent), m_fov(45.0f), m_aspect(aspectRatio) {
}

PerspectiveCamera::~PerspectiveCamera() {
}

void PerspectiveCamera::updateCamera() {
	setProjMatrix(glm::perspective(m_fov, m_aspect, getZNear(), getZFar()));
}

}
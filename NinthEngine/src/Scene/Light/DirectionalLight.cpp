#include "pch.h"
#include "DirectionalLight.h"

namespace NinthEngine {

DirectionalLight* DirectionalLight::create(Node* parent) {
	auto node = new DirectionalLight(parent);
	if (parent != nullptr) parent->addChild(node);
	return node;
}

DirectionalLight::DirectionalLight(Node* parent) : Light(parent) {
	m_data.type = static_cast<float>((int)LightType::Directional);
}

DirectionalLight::~DirectionalLight() {}

void DirectionalLight::updateLight() {
	m_data.position = glm::vec4(getWorldRotation() * glm::vec3(0.0f, 1.0f, 0.0f), 0.0f);
}

}
#include "NinthEngine/NEPCH.h"
#include "NinthEngine/Scene/Light/Light.h"

namespace NinthEngine {

Light::Light(Node* parent, const LightProps& props)
	: SceneNode(parent), m_diffuse(props.diffuse), m_specular(props.specular) {}

Light::~Light() {}

void Light::updateLocal() {
	Node::updateLocal();
	updateLight();
}

void Light::setDiffuse(const Color& diffuse) {
	m_diffuse = diffuse;
	onLocalModified();
}

void Light::setSpecular(const Color& specular) {
	m_specular = specular;
	onLocalModified();
}

}
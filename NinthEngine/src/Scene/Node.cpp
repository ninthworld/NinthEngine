#include "NinthEngine/NEPCH.h"
#include "NinthEngine/Scene/Node.h"

namespace NinthEngine {

Node::Node(Node* parent)
	: m_parent(parent), m_localModified(true)
	, m_localPosition(glm::vec3()), m_worldPosition(glm::vec3())
	, m_localScale(glm::vec3(1.0f)), m_worldScale(glm::vec3(1.0f))
	, m_localRotation(glm::quat()), m_worldRotation(glm::quat())
	, m_localTransform(glm::mat4(1.0f)), m_worldTransform(glm::mat4(1.0f)) {};

Node::~Node() {}

void Node::update() {
	if (m_localModified) {
		updateLocal();
		m_localModified = false;
	}
	for (auto it = m_children.begin(); it != m_children.end(); ++it) {
		(*it)->update();
	}
}

void Node::updateLocal() {
	m_localTransform = glm::translate(glm::mat4(1.0f), m_localPosition)
		* glm::toMat4(m_localRotation)
		* glm::scale(glm::mat4(1.0f), m_localScale);
	m_worldTransform = (m_parent != nullptr ? m_parent->getWorldTransform() : glm::mat4(1.0f)) * m_localTransform;
	glm::decompose(m_worldTransform, m_worldScale, m_worldRotation, m_worldPosition, glm::vec3(), glm::vec4());
}

Node* Node::getChild(unsigned int index) { 
	if (index >= 0 && index < getNumChildren()) return m_children[index];
	return nullptr;
}

void Node::addChild(Node* child) {
	if (child != nullptr) {
		if (child->getParent() != nullptr) child->getParent()->removeChild(child);
		m_children.push_back(child);
		child->m_parent = this;
	}
} 

void Node::removeChild(Node* child) {
	if (child != nullptr) {
		auto it = std::find(m_children.begin(), m_children.end(), child);
		if (it != m_children.end()) {
			child->m_parent = nullptr;
			m_children.erase(it);
		}
	}
};

 void Node::setLocalPosition(const glm::vec3& position) {
	m_localPosition = position;
	onLocalModified();
}

void Node::setLocalScale(const glm::vec3& scale) {
	m_localScale = scale;
	onLocalModified();
}

void Node::setLocalRotation(const glm::quat& rotation) {
	m_localRotation = rotation;
	onLocalModified();
}

glm::vec3 Node::getLocalForward() {
	return glm::normalize(glm::vec3(glm::inverse(m_localTransform)[2]));
}

glm::vec3 Node::getLocalUp() {
	return glm::normalize(glm::vec3(glm::inverse(m_localTransform)[1]));
}

glm::vec3 Node::getLocalRight() {
	return glm::normalize(glm::vec3(glm::inverse(m_localTransform)[0]));
}

glm::vec3 Node::getWorldForward() {
	return glm::normalize(glm::vec3(glm::inverse(m_worldTransform)[2]));
}

glm::vec3 Node::getWorldUp() {
	return glm::normalize(glm::vec3(glm::inverse(m_worldTransform)[1]));
}

glm::vec3 Node::getWorldRight() {
	return glm::normalize(glm::vec3(glm::inverse(m_worldTransform)[0]));
}

void Node::translate(const glm::vec3& amount) {
	m_localPosition += amount;
	onLocalModified();
}

void Node::scale(const glm::vec3& amount) {
	m_localScale *= amount;
	onLocalModified();
}

void Node::rotate(const float degree, const glm::vec3& axis) {
	glm::vec3 rot = axis * (degree * glm::pi<float>() / 180.0f);
	m_localRotation = glm::normalize(m_localRotation * glm::quat(rot));
	onLocalModified();
}

void Node::lookAt(const glm::vec3& point) {
	m_localRotation = glm::quat_cast(glm::lookAt(m_localPosition, point, glm::vec3(0.0f, 1.0f, 0.0f)));
	onLocalModified();
}

void Node::lookAlong(const glm::vec3& direction) {
	m_localRotation = glm::quat_cast(glm::lookAt(glm::vec3(), direction, glm::vec3(0.0f, 1.0f, 0.0f)));
	onLocalModified();
}

}
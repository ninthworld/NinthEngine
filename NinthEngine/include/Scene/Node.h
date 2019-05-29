#pragma once

#include "pch.h"
#include "NinthEngine/Core.h"

namespace NinthEngine {

class NE_API Node {
protected:
	Node(Node* parent)
		: m_parent(parent), m_localModified(true)
		, m_localPosition(glm::vec3()), m_worldPosition(glm::vec3())
		, m_localScale(glm::vec3(1.0f)), m_worldScale(glm::vec3(1.0f))
		, m_localRotation(glm::quat()), m_worldRotation(glm::quat()) 
		, m_localTransform(glm::mat4(1.0f)), m_worldTransform(glm::mat4(1.0f)) {};

public:
	virtual ~Node() {};

	inline virtual void update() {
		if (m_localModified) {
			updateLocal();
			m_localModified = false;
		}
		for (auto it = m_children.begin(); it != m_children.end(); ++it) {
			(*it)->update();
		}
	};

	inline virtual void updateLocal() {
		m_localTransform = glm::translate(glm::mat4(1.0f), m_localPosition)
			* glm::toMat4(m_localRotation)
			* glm::scale(glm::mat4(1.0f), m_localScale);
		m_worldTransform = (m_parent != nullptr ? m_parent->getWorldTransform() : glm::mat4(1.0f)) * m_localTransform;
		glm::decompose(m_worldTransform, m_worldScale, m_worldRotation, m_worldPosition, glm::vec3(), glm::vec4());
	}

	inline virtual Node* getParent() { return m_parent; };
	inline virtual std::vector<Node*>& getChildren() { return m_children; };

	inline virtual void addChild(Node* node) {
		if (node != nullptr) {
			if (node->getParent() != nullptr) removeChild(node);
			node->m_parent = this;
			m_children.push_back(node);
		}
	};
	
	inline virtual void removeChild(Node* node) {
		if (node != nullptr) {
			auto it = std::find(m_children.begin(), m_children.end(), node);
			if (it != m_children.end()) {
				m_children.erase(it);
				node->m_parent = nullptr;
			}
		}
	};

	inline virtual void setLocalPosition(const glm::vec3& position) {
		m_localPosition = position;
		m_localModified = true;
	};

	inline virtual void setLocalScale(const glm::vec3& scale) {
		m_localScale = scale;
		m_localModified = true;
	};

	inline virtual void setLocalRotation(const glm::quat& rotation) {
		m_localRotation = rotation;
		m_localModified = true;
	};

	inline virtual glm::vec3 getLocalPosition() { return m_localPosition; };
	inline virtual glm::vec3 getLocalScale() { return m_localScale; };
	inline virtual glm::quat getLocalRotation() { return m_localRotation; };
	inline virtual glm::mat4 getLocalTransform() { return m_localTransform; };

	inline virtual glm::vec3 getLocalForward() {
		return glm::normalize(glm::vec3(glm::inverse(m_localTransform)[2]));
	};

	inline virtual glm::vec3 getLocalUp() {
		return glm::normalize(glm::vec3(glm::inverse(m_localTransform)[1]));
	};

	inline virtual glm::vec3 getLocalRight() {
		return glm::normalize(glm::vec3(glm::inverse(m_localTransform)[0]));
	};

	inline virtual glm::vec3 getWorldPosition() { return m_worldPosition; };
	inline virtual glm::vec3 getWorldScale() { return m_worldScale; };
	inline virtual glm::quat getWorldRotation() { return m_worldRotation; };
	inline virtual glm::mat4 getWorldTransform() { return m_worldTransform; };

	inline virtual glm::vec3 getWorldForward() {
		return glm::normalize(glm::vec3(glm::inverse(m_worldTransform)[2]));
	};

	inline virtual glm::vec3 getWorldUp() {
		return glm::normalize(glm::vec3(glm::inverse(m_worldTransform)[1]));
	};

	inline virtual glm::vec3 getWorldRight() {
		return glm::normalize(glm::vec3(glm::inverse(m_worldTransform)[0]));
	};

	inline virtual void translate(const glm::vec3& amount) {
		m_localPosition += amount;
		m_localModified = true;
	};

	inline virtual void scale(const glm::vec3& amount) {
		m_localScale *= amount;
		m_localModified = true;
	};

	inline virtual void rotate(const float degree, const glm::vec3& axis) {
		glm::vec3 rot = axis * (degree * 3.141592f / 180.0f);
		m_localRotation = glm::normalize(m_localRotation * glm::quat(rot));
		m_localModified = true;
	};

	inline virtual void lookAt(const glm::vec3& point) {
		m_localRotation = glm::quat_cast(glm::lookAt(m_localPosition, point, glm::vec3(0.0f, 1.0f, 0.0f)));
		m_localModified = true;
	};

	inline virtual void lookAlong(const glm::vec3& direction) {
		m_localRotation = glm::quat_cast(glm::lookAt(glm::vec3(), direction, glm::vec3(0.0f, 1.0f, 0.0f)));
		m_localModified = true;
	};

	static Node* create(Node* parent) {
		auto node = new Node(parent);
		if (parent != nullptr) parent->addChild(node);
		return node;
	};
	
private:
	Node* m_parent;
	std::vector<Node*> m_children;

	glm::vec3 m_worldPosition;
	glm::vec3 m_worldScale;
	glm::quat m_worldRotation;
	glm::mat4 m_worldTransform;

	glm::vec3 m_localPosition;
	glm::vec3 m_localScale;
	glm::quat m_localRotation;
	glm::mat4 m_localTransform;

	bool m_localModified;
};

}
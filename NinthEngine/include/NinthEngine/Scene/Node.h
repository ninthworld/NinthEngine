#pragma once

#include "NinthEngine/NEPCH.h"
#include "NinthEngine/Core.h"

namespace NinthEngine {

class NE_API Node {
protected:
	Node(Node* parent);
public:
	virtual ~Node();

	virtual void update();

	inline virtual Node* getParent() { return m_parent; }

	virtual Node* getChild(unsigned int index);
	inline virtual const unsigned int getNumChildren() const { return m_children.size(); }

	virtual void addChild(Node* child);	
	virtual void removeChild(Node* child);

	virtual void setLocalPosition(const glm::vec3& position);
	virtual void setLocalScale(const glm::vec3& scale);
	virtual void setLocalRotation(const glm::quat& rotation);

	inline virtual glm::vec3 getLocalPosition() { return m_localPosition; }
	inline virtual glm::vec3 getLocalScale() { return m_localScale; }
	inline virtual glm::quat getLocalRotation() { return m_localRotation; }
	inline virtual glm::mat4 getLocalTransform() { return m_localTransform; }

	inline virtual glm::vec3 getWorldPosition() { return m_worldPosition; }
	inline virtual glm::vec3 getWorldScale() { return m_worldScale; }
	inline virtual glm::quat getWorldRotation() { return m_worldRotation; }
	inline virtual glm::mat4 getWorldTransform() { return m_worldTransform; }

	virtual glm::vec3 getLocalForward();
	virtual glm::vec3 getLocalUp();
	virtual glm::vec3 getLocalRight();

	virtual glm::vec3 getWorldForward();
	virtual glm::vec3 getWorldUp();
	virtual glm::vec3 getWorldRight();

	virtual void translate(const glm::vec3& amount);
	virtual void scale(const glm::vec3& amount);
	virtual void rotate(const float degree, const glm::vec3& axis);
	virtual void lookAt(const glm::vec3& point);
	virtual void lookAlong(const glm::vec3& direction);
	
protected:
	virtual void updateLocal();
	virtual void onLocalModified() { m_localModified = true; }

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
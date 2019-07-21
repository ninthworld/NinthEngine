#include "NinthEngine/NEPCH.h"
#include "NinthEngine/Scene/SceneNode.h"
#include "NinthEngine/Scene/SceneManager.h"
#include "NinthEngine/Scene/Camera/PerspectiveCamera.h"
#include "NinthEngine/Scene/Light/PointLight.h"
#include "NinthEngine/Scene/Light/DirectionalLight.h"

namespace NinthEngine {

SceneNode* SceneNode::createSceneNode() {
	SceneNode* child = new SceneNode(this);
	addChild(child);
	child->m_sceneManager = m_sceneManager;
	m_sceneManager->m_allNodes.push_back(child);
	return child;
}

PerspectiveCamera* SceneNode::createPerspectiveCamera(const PerspectiveCameraProps& props) {
	PerspectiveCamera* child = new PerspectiveCamera(this, props);
	addChild(child);
	child->m_sceneManager = m_sceneManager;
	m_sceneManager->getNodes().push_back(child);
	m_sceneManager->getCameras().push_back(child);
	return child;
}

Light* SceneNode::createAmbientLight(const LightProps& props) {
	Light* child = new Light(this, props);
	addChild(child);
	child->m_sceneManager = m_sceneManager;
	m_sceneManager->getNodes().push_back(child);
	m_sceneManager->getLights().push_back(child);
	return child;
}

PointLight* SceneNode::createPointLight(const PointLightProps& props) {
	PointLight* child = new PointLight(this, props);
	addChild(child);
	child->m_sceneManager = m_sceneManager;
	m_sceneManager->getNodes().push_back(child);
	m_sceneManager->getLights().push_back(child);
	return child;
}

DirectionalLight* SceneNode::createDirectionalLight(const DirectionalLightProps& props) {
	DirectionalLight* child = new DirectionalLight(this, props);
	addChild(child);
	child->m_sceneManager = m_sceneManager;
	m_sceneManager->getNodes().push_back(child);
	m_sceneManager->getLights().push_back(child);
	return child;
}

}
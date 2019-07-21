#include "NinthEngine/NEPCH.h"
#include "NinthEngine/Scene/SceneManager.h"
#include "NinthEngine/Scene/SceneNode.h"
#include "NinthEngine/Scene/Camera/Camera.h"
#include "NinthEngine/Scene/Light/Light.h"

namespace NinthEngine {

SceneManager::SceneManager() {
	m_root = new SceneNode(nullptr);
	m_root->m_sceneManager = this;
	m_allNodes.push_back(m_root);
}

SceneManager::~SceneManager() {
	for (auto it = m_allNodes.begin(); it != m_allNodes.end(); ++it) {
		delete *it;
	}
}

void SceneManager::update() {
	m_root->update();
}

Camera* SceneManager::getCamera(unsigned int index) {
	if (index >= 0 && index < getNumCameras()) return m_cameras[index];
	return nullptr;
}

Light* SceneManager::getLight(unsigned int index) {
	if (index >= 0 && index < getNumLights()) return m_lights[index];
	return nullptr;
}

}
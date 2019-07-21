#pragma once

#include "NinthEngine/NEPCH.h"
#include "NinthEngine/Core.h"
#include "Node.h"

namespace NinthEngine {

class SceneNode;
class Camera;
class Light;

class NE_API SceneManager {
	friend class SceneNode;
public:
	SceneManager();
	~SceneManager();

	void update();

	inline SceneNode* getRoot() { return m_root; }

	Camera* getCamera(unsigned int index);
	inline const unsigned int getNumCameras() const { return m_cameras.size(); }

	Light* getLight(unsigned int index);
	inline const unsigned int getNumLights() const { return m_lights.size(); }
	
protected:
	std::vector<Camera*>& getCameras() { return m_cameras; }
	std::vector<Light*>& getLights() { return m_lights; }
	std::vector<Node*>& getNodes() { return m_allNodes; }

private:
	std::vector<Camera*> m_cameras;
	std::vector<Light*> m_lights;
	std::vector<Node*> m_allNodes;

private:
	SceneNode* m_root;
};

}
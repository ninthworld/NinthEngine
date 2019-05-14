#pragma once

#include "pch.h"
#include "NinthEngine/Core.h"

namespace NinthEngine {

class SceneNode;
class Camera;
class Light;

class NE_API Scene : public std::enable_shared_from_this<Scene> {
public:
	Scene();
	~Scene();

	void update();

	inline std::shared_ptr<SceneNode> getRoot() { return m_rootNode; };
	inline std::vector<std::shared_ptr<Camera>> getCameras() { return m_cameras; };
	inline std::vector<std::shared_ptr<Light>> getLights() { return m_lights; };

	void addCamera(const std::shared_ptr<Camera>& camera);
	void removeCamera(const unsigned int index);
	void removeCamera(const std::shared_ptr<Camera>& camera);

private:
	std::shared_ptr<SceneNode> m_rootNode;
	std::vector<std::shared_ptr<Camera>> m_cameras;
	std::vector<std::shared_ptr<Light>> m_lights;
};

}
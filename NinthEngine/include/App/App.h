#pragma once

#include "NEPCH.h"

#include "Core.h"
#include "Window\Window.h"
#include "Graphics\Graphics.h"
#include "Scene\Scene.h"
#include "Resource\ResourceLoader.h"

namespace NinthEngine {

class NE_API App {
public:
	App();
	App(const unsigned int width, const unsigned int height);
	App(const std::string& title, const unsigned int width, const unsigned int height);
	App(const std::string& title, const unsigned int width, const unsigned int height, const std::string& graphicsAPI);
	virtual ~App();

	void run();
	
	virtual void initialize();
	virtual void update(const double deltaTime);
	virtual void render();

	Graphics* getGraphics();
	Window* getWindow();
	Scene* getScene();
	ResourceLoader* getResourceLoader();

private:
	std::unique_ptr<Graphics> m_graphics;
	std::unique_ptr<Window> m_window;
	std::unique_ptr<Scene> m_scene;
	std::unique_ptr<ResourceLoader> m_resourceLoader;

	bool m_running;
};

static App* createApp();

}
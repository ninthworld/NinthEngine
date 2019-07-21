#include "NinthEngine/NEPCH.h"
#include "NinthEngine/Application.h"

namespace NinthEngine {

Application::Application(const ApplicationProps& props) {
	m_window = std::unique_ptr<Window>(Window::create(props.windowProps));
	m_graphics = std::unique_ptr<Graphics>(props.graphicsFunc(props.graphicsProps, m_window.get()));
	m_sceneManager = std::unique_ptr<SceneManager>(new SceneManager());
	m_resourceManager = std::unique_ptr<ResourceManager>(new ResourceManager(m_graphics.get()));

	m_running = true;
}

Application::~Application() {
}

void Application::create() {
}

void Application::update() {
}

Graphics* Application::getGraphics() {
	return m_graphics.get();
}

Window* Application::getWindow() {
	return m_window.get();
}

SceneManager* Application::getSceneManager() {
	return m_sceneManager.get();
}

ResourceManager* Application::getResourceManager() {
	return m_resourceManager.get();
}

}

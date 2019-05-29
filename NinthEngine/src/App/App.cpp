#include "NEPCH.h"

#include "App\App.h"
#include "Graphics\GraphicsAPI.h"

namespace NinthEngine {

App::App() : App(640, 480) {}

App::App(const unsigned int width, const unsigned int height) : App("NinthEngine", width, height) {}

App::App(const std::string& title, const unsigned int width, const unsigned int height) : App(title, width, height, "OpenGL") {}

App::App(const std::string& title, const unsigned int width, const unsigned int height, const std::string& graphicsAPI) {
	m_graphics = std::unique_ptr<Graphics>(GraphicsAPI::getGraphics(graphicsAPI));
	m_window = std::unique_ptr<Window>(m_graphics->createWindow(title, width, height));
	m_scene = std::unique_ptr<Scene>(new Scene());
	m_resourceLoader = std::unique_ptr<ResourceLoader>(new ResourceLoader());
	m_running = true;
}

App::~App() {}

void App::run() {
	initialize();
	while (m_running) {
		update(1.0);
		m_scene->update();

		render();
		m_graphics->swapBuffers();

		m_window->onUpdate();
	}
}

void App::initialize() {
}

void App::update(const double deltaTime) {
}

void App::render() {
}

Graphics* App::getGraphics() {
	return m_graphics.get();
}

Window* App::getWindow() {
	return m_window.get();
}

Scene* App::getScene() {
	return m_scene.get();
}

ResourceLoader* App::getResourceLoader() {
	return m_resourceLoader.get();
}

}

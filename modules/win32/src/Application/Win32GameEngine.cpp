#ifdef _WIN32

#include <NinthEngine\Utils\LogUtils.hpp>
#include "..\..\include\NinthEngineWin32\Win32Bootstrap.hpp"
#include "Win32GameEngine.hpp"

namespace NinthEngine {
namespace Win32 {

Win32GameEngine::Win32GameEngine(
	const std::shared_ptr<Win32GameWindow>& window, 
	const std::shared_ptr<RenderEngine>& renderEngine)
	: m_window(window) {

	renderEngine->init(window, window->getHandle());
	m_device = renderEngine->getGraphicsDevice();
	m_context = renderEngine->getGraphicsContext();
	
	m_timer = std::make_unique<Win32GameTimer>();
	m_context->setViewport({ 0, 0, window->getWidth(), window->getHeight() });
}

Win32GameEngine::~Win32GameEngine() {

	m_device.reset();
	m_context.reset();
	m_timer.reset();
}

void Win32GameEngine::run(std::unique_ptr<Game> game) {
	
	m_window->setResizeCallback(
		[&game](const int width, const int height) {
		game->onResize(width, height); });

	auto win32Keyboard = std::dynamic_pointer_cast<Win32Keyboard>(m_window->getKeyboard());
	win32Keyboard->setKeyCallback(
		[&game](const Key key, const KeyState state) {
		game->onKeyboard(key, state); });

	auto win32Mouse = std::dynamic_pointer_cast<Win32Mouse>(m_window->getMouse());
	win32Mouse->setButtonCallback(
		[&game](const MouseButton button, const MouseState state) {
		game->onMouseButton(button, state); });

	win32Mouse->setMoveCallback(
		[&game](const int x, const int y) {
		game->onMouseMove(x, y); });

	game->init();

	const std::string title = m_window->getTitle();
	double deltaTime = 0.0;
	int frames = 0;

	while (!m_window->isClosed()) {

		deltaTime = m_timer->elapsed();
		m_timer->reset();

		game->update(deltaTime);
		game->render();
		m_context->swapBuffers();
		
		m_window->update();
		frames++;
	}

	game.reset();
}

std::unique_ptr<GameTimer> Win32GameEngine::createTimer() {
	
	return std::make_unique<Win32GameTimer>();
}

} // namespace Win32
} // namespace NinthEngine

#endif
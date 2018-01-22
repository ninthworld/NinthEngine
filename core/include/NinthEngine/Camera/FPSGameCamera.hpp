#pragma once

#include "..\Input\Keyboard.hpp"
#include "..\Input\Mouse.hpp"
#include "..\Application\GameWindow.hpp"
#include "GameCamera.hpp"

namespace NinthEngine {

class FPSGameCamera : public GameCamera {
public:
	FPSGameCamera(
		const glm::vec3 position = glm::vec3(0),
		const glm::vec3 rotation = glm::vec3(0),
		const float moveFactor = 8.0f,
		const float yawFactor = 0.001f,
		const float pitchFactor = 0.001f,
		const float fov = 45.0f,
		const float zNear = 0.1f,
		const float zFar = 1000.0f,
		const int width = 1,
		const int height = 1);
	~FPSGameCamera();

	void update(const double deltaTime) override;

	void onKeyboard(const Key key, const KeyState state);
	void onMouseButton(const std::shared_ptr<GameWindow>& window, const MouseButton button, const MouseState state);
	void onMouseMove(const std::shared_ptr<GameWindow>& window, const int x, const int y);

protected:
private:
	float m_moveFactor;
	float m_yawFactor;
	float m_pitchFactor;

	glm::vec2 m_mouseDelta;
	std::array<MouseState, MB_NB> m_mouseState;
	std::array<KeyState, KEY_NB> m_keyState;

};

} // namespace NinthEngine
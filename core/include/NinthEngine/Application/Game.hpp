#pragma once

#include <memory>
#include "GameWindow.hpp"
#include "GameEngine.hpp"
#include "GameTimer.hpp"
#include "..\Input\Keyboard.hpp"
#include "..\Input\Mouse.hpp"
#include "..\Camera\GameCamera.hpp"
#include "..\Render\Buffer.hpp"
#include "..\Render\GraphicsContext.hpp"
#include "..\Render\GraphicsDevice.hpp"
#include "..\Render\Rasterizer.hpp"
#include "..\Render\RenderTarget.hpp"
#include "..\Render\Texture.hpp"
#include "..\Render\Sampler.hpp"
#include "..\Utils\MathUtils.hpp"

namespace NinthEngine {

class Game {
public:
	Game() = default;
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

	virtual ~Game() = default;

	virtual void init() = 0;
	virtual void update(const double deltaTime) = 0;
	virtual void render() = 0;

	virtual void onResize(const int width, const int height) {};
	virtual void onKeyboard(const Key key, const KeyState state) {};
	virtual void onMouseButton(const MouseButton button, const MouseState state) {};
	virtual void onMouseMove(const int x, const int y) {};

protected:
private:
};

} // namespace NinthEngine
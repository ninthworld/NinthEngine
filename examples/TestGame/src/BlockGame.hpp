#pragma once

#include <NinthEngine\IGameLogic.hpp>
#include <NinthEngine\Texture.hpp>
#include <NinthEngine\Camera.hpp>

using namespace NinthEngine;

namespace TestGame {

class SimpleShader;
class Chunk;

class BlockGame : public IGameLogic {
public:
	BlockGame();
	~BlockGame();
	void init(Window *window);
	void input(Window *window);
	void update(const float interval);
	void render(Window *window);
protected:
private:
	SimpleShader *simpleShader;
	//VertexArray *simpleVAO;
	Camera *camera;
	Texture *simpleTexture;
	Chunk *chunk;
};

} // namespace TestGame
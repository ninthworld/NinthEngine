#pragma once

#include <NinthEngine\IGameLogic.hpp>
#include <NinthEngine\Texture.hpp>
#include <NinthEngine\Camera.hpp>
#include <NinthEngine\VertexArray.hpp>
#include "SimpleShader.hpp"
#include "Chunk.hpp"

using namespace NinthEngine;

namespace TestGame {

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
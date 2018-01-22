#pragma once

#include <memory>
#include <NinthEngine\Application\Game.hpp>

using namespace NinthEngine;

const static LayoutConfig shadowMapStructLayout =
	LayoutConfig().float4x4();
struct ShadowMapStruct {
	glm::mat4 shadowViewProj;
};
/*
* GLSL
	layout(std140) uniform ShadowMap {
		mat4 shadowViewProj;
	};

* HLSL
	cbuffer ShadowMap : register(b?) {
		float4x4 shadowViewProj;
	};
*/

class ShadowMap {
public:
	ShadowMap(
		const std::shared_ptr<GraphicsDevice>& device,
		const std::shared_ptr<GraphicsContext>& context);
	~ShadowMap();

	void update(
		const std::shared_ptr<GameWindow>& window, 
		const std::shared_ptr<GameCamera>& camera, 
		const glm::vec3 sunPosition);

	std::shared_ptr<ConstantBuffer> getConstantShadowMap() { return m_constantShadowMap; };

private:
	// Constants
	std::shared_ptr<ConstantBuffer> m_constantShadowMap;
	void initConstants(const std::shared_ptr<GraphicsDevice>& device);

private:
	std::shared_ptr<GraphicsContext> m_context;

};
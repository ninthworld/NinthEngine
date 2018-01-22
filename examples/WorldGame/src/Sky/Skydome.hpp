#pragma once

#include <memory>
#include <NinthEngine\Application\Game.hpp>

using namespace NinthEngine;

const static LayoutConfig skydomeLayout =
	LayoutConfig().float3(POSITION).float2(TEXCOORD);

const static LayoutConfig skyStructLayout =
	LayoutConfig().float4().float4();
struct SkyStruct {
	glm::vec4 sunPosition;
	glm::vec4 skyColor;
};
/*
* GLSL
	layout(std140) uniform Sky {
		vec4 sunPosition;
		vec4 skyColor;
	};

* HLSL
	cbuffer Sky : register(b?) {
		float4 sunPosition;
		float4 skyColor;
	};
*/

class Skydome {
public:
	Skydome(
		const std::shared_ptr<GraphicsDevice>& device,
		const std::shared_ptr<GraphicsContext>& context,
		const std::shared_ptr<ConstantBuffer>& constantCamera);
	~Skydome();

	void update();
	void render();
	
	void onKeyboard(const Key key, const KeyState state);

	std::shared_ptr<ConstantBuffer> getConstantSky() { return m_constantSky; };

	const SkyStruct getSkyStruct() const { return m_skyStruct; };

private:
	// Geometry
	std::shared_ptr<IndexBuffer> m_indexBuffer;
	std::shared_ptr<VertexArray> m_vertexArray;
	void initGeometry(const std::shared_ptr<GraphicsDevice>& device);

	// Constants
	std::shared_ptr<ConstantBuffer> m_constantSky;
	void initConstants(const std::shared_ptr<GraphicsDevice>& device);

	// Shaders
	std::shared_ptr<Shader> m_shader;
	void initShaders(
		const std::shared_ptr<GraphicsDevice>& device,
		const std::shared_ptr<ConstantBuffer>& constantCamera);

private:
	std::shared_ptr<GraphicsContext> m_context;

	float m_sunTheta;
	SkyStruct m_skyStruct;

};
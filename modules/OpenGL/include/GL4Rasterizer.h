#pragma once

#include "pch.h"
#include "NinthEngine/Graphics/Rasterizer.h"
#include "GL4Utils.h"

namespace NinthEngine {

class GL4Rasterizer : public Rasterizer, GL4Object {
public:
	GL4Rasterizer(
		const RasterizerProps& props,
		std::function<void(GL4Rasterizer*)> pushRasterizer,
		std::function<GL4Rasterizer*(void)> popRasterizer);
	~GL4Rasterizer();
	
	void bind() override;
	void unbind() override;

	inline const RasterizerFillType getFillType() const override { return m_fill; };
	inline const RasterizerCullType getCullType() const override { return m_cull; };
	inline const bool isDepthTest() const override { return m_depthTest; };
	inline const bool isFrontCCW() const override { return m_frontCCW; };
	inline const bool isMultisampling() const override { return m_multisampling; };
	inline const bool isScissoring() const override { return m_scissoring; };
	inline const bool isLineAntialiasing() const override { return m_lineAntialiasing; };

	void enable();

private:
	std::function<void(GL4Rasterizer*)> m_pushRasterizer;
	std::function<GL4Rasterizer*(void)> m_popRasterizer;

	RasterizerFillType m_fill;
	RasterizerCullType m_cull;
	bool m_depthTest, m_frontCCW, m_multisampling, m_scissoring, m_lineAntialiasing;
};

}
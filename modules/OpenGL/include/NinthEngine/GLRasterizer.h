#pragma once

#include <NinthEngine/Graphics/Rasterizer.h>
#include "GLObject.h"
#include "GLStateManager.h"

namespace NinthEngine {

class NE_GL_API GLRasterizer : public Rasterizer, GLObject {
public:
	GLRasterizer(const RasterizerProps& props, GLStateManager<GLRasterizer>* stateManager);
	~GLRasterizer();

	void bind() override;
	void unbind() override;

	inline const RasterizerFillType getFillType() const override { return m_fill; }
	inline const RasterizerCullType getCullType() const override { return m_cull; }
	inline const bool isDepthTest() const override { return m_depthTest; }
	inline const bool isFrontCCW() const override { return m_frontCCW; }
	inline const bool isMultisampling() const override { return m_multisampling; }
	inline const bool isScissoring() const override { return m_scissoring; }
	inline const bool isLineAntialiasing() const override { return m_lineAntialiasing; }
	
	inline const GLuint getNativeId() const override { return 0; }
	void destroy() override {};

private:
	GLStateManager<GLRasterizer>* m_stateManager;
	RasterizerFillType m_fill;
	RasterizerCullType m_cull;
	bool m_depthTest, m_frontCCW, m_multisampling, m_scissoring, m_lineAntialiasing;
};

}
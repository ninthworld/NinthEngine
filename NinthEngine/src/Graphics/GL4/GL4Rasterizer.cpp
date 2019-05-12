#include "pch.h"
#include "GL4Rasterizer.h"

namespace NinthEngine {

GL4Rasterizer::GL4Rasterizer(
	const RasterizerProps& props,
	std::function<void(const std::shared_ptr<GL4Rasterizer>&)> pushRasterizer,
	std::function<std::shared_ptr<GL4Rasterizer>(void)> popRasterizer)
	: m_fill(props.fill), m_cull(props.cull)
	, m_depthTest(props.depthTest), m_frontCCW(props.frontCCW)
	, m_multisampling(props.multisampling), m_scissoring(props.scissoring)
	, m_lineAntialiasing(props.lineAntialiasing)
	, m_pushRasterizer(pushRasterizer), m_popRasterizer(popRasterizer) {
}

GL4Rasterizer::~GL4Rasterizer() {
}

void GL4Rasterizer::bind() {
	m_pushRasterizer(shared_from_this());
	enable();
}

void GL4Rasterizer::unbind() {
	m_popRasterizer()->enable();
}

void GL4Rasterizer::enable() {
	glPolygonMode(GL_FRONT_AND_BACK, (m_fill == RasterizerFillType::Solid ? GL_FILL : GL_LINE));
	glFrontFace((m_frontCCW ? GL_CCW : GL_CW));

	if (m_cull != RasterizerCullType::None) {
		glEnable(GL_CULL_FACE);
		glCullFace(m_cull == RasterizerCullType::Front ? GL_FRONT : GL_BACK);
	}
	else {
		glDisable(GL_CULL_FACE);
	}

	if (m_depthTest) {
		glEnable(GL_DEPTH_TEST);
	}
	else {
		glDisable(GL_DEPTH_TEST);
	}

	if (m_multisampling) {
		glEnable(GL_MULTISAMPLE);
	}
	else {
		glDisable(GL_MULTISAMPLE);
	}

	if (m_scissoring) {
		glEnable(GL_SCISSOR_TEST);
	}
	else {
		glDisable(GL_SCISSOR_TEST);
	}

	if (m_lineAntialiasing) {
		glEnable(GL_LINE_SMOOTH);
		glLineWidth(0.5f);
	}
	else {
		glDisable(GL_LINE_SMOOTH);
	}
}

}
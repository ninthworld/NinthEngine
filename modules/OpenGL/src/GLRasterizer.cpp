#include "NinthEngine/GLPCH.h"
#include "NinthEngine/GLRasterizer.h"

namespace NinthEngine {

GLRasterizer::GLRasterizer(const RasterizerProps& props, GLStateManager<GLRasterizer>* stateManager)
	: m_fill(props.fill), m_cull(props.cull)
	, m_depthTest(props.depthTest), m_frontCCW(props.frontCCW)
	, m_multisampling(props.multisampling), m_scissoring(props.scissoring)
	, m_lineAntialiasing(props.lineAntialiasing)
	, m_stateManager(stateManager) {
}

GLRasterizer::~GLRasterizer() {
}

void GLRasterizer::bind() {
	m_stateManager->push(this);

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

void GLRasterizer::unbind() {
	m_stateManager->pop()->bind();
}

}
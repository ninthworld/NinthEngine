#include <plog\Log.h>
#include "GLRasterizer.hpp"

namespace NinthEngine {
namespace GL {

GLRasterizer::GLRasterizer(const RasterizerConfig& config)
	: m_config(config) {
}

GLRasterizer::~GLRasterizer() {
}

void GLRasterizer::bind() {
	/*
	glEnable(GL_CLIP_DISTANCE0);
	glEnable(GL_CLIP_DISTANCE1);
	glEnable(GL_CLIP_DISTANCE2);
	glEnable(GL_CLIP_DISTANCE3);
	glEnable(GL_CLIP_DISTANCE4);
	glEnable(GL_CLIP_DISTANCE5);
	*/

	glPolygonMode(GL_FRONT_AND_BACK, (m_config.m_config.m_fillMode ? GL_LINE : GL_FILL));

	glFrontFace((m_config.m_config.m_frontCCW ? GL_CCW : GL_CW));

	if (m_config.m_config.m_cullMode != CULL_NONE) {
		glEnable(GL_CULL_FACE);
		glCullFace((m_config.m_config.m_cullMode == CULL_FRONT ? GL_FRONT : GL_BACK));
	}
	else {
		glDisable(GL_CULL_FACE);
	}

	if (m_config.m_config.m_depthClipping) {
		glEnable(GL_DEPTH_TEST);
		//glDepthRange(m_config.m_depthBias, m_config.m_depthBiasSlopeScaled * m_config.m_depthBiasClamp);
	}
	else {
		glDisable(GL_DEPTH_TEST);
	}

	if (m_config.m_config.m_multisampling) {
		glEnable(GL_MULTISAMPLE);
	}
	else {
		glDisable(GL_MULTISAMPLE);
	}

	if (m_config.m_config.m_scissoring) {
		glEnable(GL_SCISSOR_TEST);
	}
	else {
		glDisable(GL_SCISSOR_TEST);
	}	

	if (m_config.m_config.m_lineAntialiasing) {
		glEnable(GL_LINE_SMOOTH);
		glLineWidth(0.5f);
	}
	else {
		glDisable(GL_LINE_SMOOTH);
	}
}

} // namespace GL
} // namespace NinthEngine
#include <NinthEngine.h>

class Test : public NinthEngine::App {
public:
	Test() : NinthEngine::App(NinthEngine::WindowProps("TestApp", 640, 480), NinthEngine::GraphicsProps(NinthEngine::OpenGL4)) {
	}

	~Test() {
	}

	void Test::initialize() {
		NinthEngine::ShaderProps triShaderProps;
		NinthEngine::ShaderProps quadShaderProps;
		if (getGraphics()->getGraphicsAPI() == NinthEngine::GraphicsAPI::OpenGL4) {
			triShaderProps.vertexShaderSrc = 
				"#version 400\n"
				"void main(){"
				"if(gl_VertexID==0) gl_Position = vec4(0.0, 0.5, 0.0, 1.0);"
				"if(gl_VertexID==1) gl_Position = vec4(-.5, -.5, 0.0, 1.0);"
				"if(gl_VertexID==2) gl_Position = vec4(0.5, -.5, 0.0, 1.0);}";
			triShaderProps.pixelShaderSrc = 
				"#version 400\n"
				"out vec4 out_color;"
				"void main(){"
				"out_color = vec4(0.0, 1.0, 0.0, 1.0);}";
			quadShaderProps.vertexShaderSrc =
				"#version 400\n"
				"out vec2 vs_texCoord;"
				"void main(){"
				"if(gl_VertexID==0){ gl_Position = vec4(-1.0, -1.0, 0.0, 1.0); vs_texCoord = vec2(0.0, 0.0); }"
				"if(gl_VertexID==1){ gl_Position = vec4(1.0, -1.0, 0.0, 1.0); vs_texCoord = vec2(1.0, 0.0); }"
				"if(gl_VertexID==2){ gl_Position = vec4(-1.0, 1.0, 0.0, 1.0); vs_texCoord = vec2(0.0, 1.0); }"
				"if(gl_VertexID==3){ gl_Position = vec4(-1.0, 1.0, 0.0, 1.0); vs_texCoord = vec2(0.0, 1.0); }"
				"if(gl_VertexID==4){ gl_Position = vec4(1.0, 1.0, 0.0, 1.0); vs_texCoord = vec2(1.0, 1.0); }"
				"if(gl_VertexID==5){ gl_Position = vec4(1.0, -1.0, 0.0, 1.0); vs_texCoord = vec2(1.0, 0.0); }}";
			quadShaderProps.pixelShaderSrc =
				"#version 420\n"
				"in vec2 vs_texCoord;"
				"out vec4 out_color;"
				"layout(binding=0) uniform sampler2D triTexture;"
				"void main(){"
				"out_color = vec4(texture(triTexture, vs_texCoord).rgb + vec3(vs_texCoord * 0.5 + 0.5, 0.0), 1.0);}";
		}

		m_triShader = std::shared_ptr<NinthEngine::Shader>(getGraphics()->createShader(triShaderProps));
		m_quadShader = std::shared_ptr<NinthEngine::Shader>(getGraphics()->createShader(quadShaderProps));

		auto textureProps = NinthEngine::TextureProps(getWindow()->getWidth(), getWindow()->getHeight());
		auto texture = std::shared_ptr<NinthEngine::Texture>(getGraphics()->createTexture(textureProps));

		m_renderTarget = std::shared_ptr<NinthEngine::RenderTarget>(getGraphics()->createRenderTarget());
		m_renderTarget->addColorTexture(0, texture);

		m_quadShader->addTexture(0, texture);
	}

	void Test::render() {
		getGraphics()->clearBackBuffer();

		m_renderTarget->bind();
		m_renderTarget->clear();

		m_triShader->bind();
		getGraphics()->draw(3);

		m_renderTarget->unbind();

		m_quadShader->bind();
		getGraphics()->draw(6);
	}

	bool Test::onKeyPressed(NinthEngine::KeyPressedEvent& e) override {
		if (e.getKeyCode() == NE_KEY_ENTER) {
			NE_INFO("Enter was pressed");
		}

		return NinthEngine::App::onKeyPressed(e);
	}

private:
	std::shared_ptr<NinthEngine::Shader> m_triShader;
	std::shared_ptr<NinthEngine::Shader> m_quadShader;
	std::shared_ptr<NinthEngine::RenderTarget> m_renderTarget;
};

NinthEngine::App* NinthEngine::createApp() {
	return new Test();
}
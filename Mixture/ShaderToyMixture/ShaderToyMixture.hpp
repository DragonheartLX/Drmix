#pragma once
#include <Drmix.hpp>

class ShaderToyMixture: public Drmix::Mixture
{
public:
	ShaderToyMixture();
	~ShaderToyMixture();

	void onEnter() override;
	void update() override;
	void fixedUpdate(double deltaTime) override;
	void imGuiRender() override;
	void render() override;
	void onExit() override;

private:
	Drmix::OpenGL::VertexArrayBuffer* m_VAO;
	Drmix::OpenGL::VertexBuffer* m_VBO;
	Drmix::OpenGL::IndexBuffer* m_EBO;
	Drmix::OpenGL::Shader* m_Shader;
};

Drmix::Mixture* Drmix::createMixture(int argc, char** argv)
{
	return new ShaderToyMixture();
};
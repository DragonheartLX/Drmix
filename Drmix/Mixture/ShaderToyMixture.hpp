#pragma once

#include "Mixture.hpp"
#include "OpenGL/Shader/Shader.hpp"

class ShaderToyMixture:public Mixture
{
public:
	ShaderToyMixture();
	~ShaderToyMixture();

	void event(GLFWwindow* win) override;
	void update(double deltaTime) override;
	void render() override;
	void imguiRender() override;
private:

	double m_vertices[12];
	unsigned int m_indices[6];
	unsigned int m_VAO, m_VBO, m_EBO;
	Shader* m_shader;
};
#pragma once
#include "Mixture.hpp"
#include "Shader/Shader.hpp"

class Texture2DMixture: public Mixture
{
public:
	Texture2DMixture();
	~Texture2DMixture();

	void update(double deltaTime) override;
	void render() override;
	void imguiRender() override;

private:
	double m_vertices[32];
	unsigned int m_indices[6];
	unsigned int m_VAO, m_VBO, m_EBO, m_texture[2];
	Shader m_shader;
};
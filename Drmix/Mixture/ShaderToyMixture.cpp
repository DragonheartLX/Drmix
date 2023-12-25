#include "ShaderToyMixture.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>

ShaderToyMixture::ShaderToyMixture() :
	m_vertices
		{
			-1.0f,  1.0f, 0.0f,
			-1.0f, -1.0f, 0.0f,
			 1.0f, -1.0f, 0.0f,
			 1.0f,  1.0f, 0.0f
		},
	m_indices
		{
			0, 1, 2,
			0, 2, 3
		}
{
	m_shader = new Shader("../../../Drmix/res/Shader/ShaderToy.vert", "../../../Drmix/res/Shader/ShaderToy.frag");

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), m_indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), (void*)0);
	glEnableVertexAttribArray(0);
}

ShaderToyMixture::~ShaderToyMixture()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);

	delete m_shader;
}

void ShaderToyMixture::event(GLFWwindow* win)
{
	if (glfwGetKey(win, GLFW_KEY_R) == GLFW_PRESS)
	{
		delete m_shader;
		m_shader = new Shader("../../../Drmix/res/Shader/ShaderToy.vert", "../../../Drmix/res/Shader/ShaderToy.frag");
	}
}

void ShaderToyMixture::update(double deltaTime)
{
}

void ShaderToyMixture::render()
{
	m_shader->enable();
	m_shader->setUniform1f("iTime", (float)glfwGetTime());

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void ShaderToyMixture::imguiRender()
{
	if (ImGui::Button("Reload shader"))
	{
		delete m_shader;
		m_shader = new Shader("../../../Drmix/res/Shader/ShaderToy.vert", "../../../Drmix/res/Shader/ShaderToy.frag");
	}
}

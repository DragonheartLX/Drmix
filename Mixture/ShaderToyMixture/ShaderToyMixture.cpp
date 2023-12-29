#include "ShaderToyMixture.hpp"

ShaderToyMixture::ShaderToyMixture():
	m_VAO(nullptr),
	m_VBO(nullptr),
	m_EBO(nullptr),
	m_Shader(nullptr)
{
	info.name = "ShaderToyMixture";
	info.resourcePath = "../../../../Mixture/ShaderToyMixture";
	info.imGuiRender = true;
}

ShaderToyMixture::~ShaderToyMixture()
{

}

void ShaderToyMixture::onEnter()
{
	double vertices[] =
	{
		-1.0f,  1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f
	};

	unsigned int indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};

	m_VAO = new Drmix::OpenGL::VertexArrayBuffer();
	m_VBO = new Drmix::OpenGL::VertexBuffer(vertices, 3 * 4 * sizeof(double));
	m_EBO = new Drmix::OpenGL::IndexBuffer(indices, 6);
	m_Shader = new Drmix::OpenGL::Shader("ShaderToy.vert", "ShaderToy.frag");

	Drmix::OpenGL::VertexBufferLayout layout;
	layout.push<double>(3);

	m_VAO->addBuffer(*m_VBO, layout);
	m_Shader->bind();
}

void ShaderToyMixture::update()
{

}

void ShaderToyMixture::fixedUpdate(double deltaTime)
{
	m_Shader->bind();
	m_Shader->setUniform1f("iTime", (float)getTime());
}

void ShaderToyMixture::imGuiRender()
{
	if (ImGui::Button("Exit"))
		info.mixtureShouldClose = true;

	if (ImGui::Button("Reload Shader"))
	{
		m_Shader->reload("ShaderToy.vert", "ShaderToy.frag");
	}

	ImGui::Text("FPS: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

void ShaderToyMixture::render()
{
	mixtureRenderer.render(*m_VAO, *m_EBO, *m_Shader);
}

void ShaderToyMixture::onExit()
{
	delete m_VAO;
	delete m_VBO;
	delete m_EBO;
	delete m_Shader;
}
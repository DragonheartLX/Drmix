#include "ShaderToyMixture.hpp"

ShaderToyMixture::ShaderToyMixture()
{
	setRenderImGui();
}

ShaderToyMixture::~ShaderToyMixture()
{
	for (std::pair obj : objects)
		delete obj.second;
}

void ShaderToyMixture::onEnter()
{
	objects.insert(std::pair("Canvas", new Canvas()));
}

void ShaderToyMixture::update()
{
	auto it = objects.find("Canvas");
	if (it != objects.end())
	{
		it->second->shader->bind();
		it->second->shader->setUniform1f("iTime", (float)getTime());
	}
}

void ShaderToyMixture::fixedUpdate(double deltaTime)
{
}

void ShaderToyMixture::imGuiRender()
{
	if (ImGui::Button("Exit"))
		setMixtureShouldClose();

	if (ImGui::Button("Reload Shader"))
	{
		auto it = objects.find("Canvas");
		if (it != objects.end())
		{
			it->second->shader->reload("../../../../Mixture/ShaderToyMixture/ShaderToy.vert", "../../../../Mixture/ShaderToyMixture/ShaderToy.frag");
		}
	}

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

Canvas::Canvas()
{
	setRenderStatus();
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

	VAO = std::make_unique<Drmix::OpenGL::VertexArrayBuffer>();
	VBO = std::make_unique<Drmix::OpenGL::VertexBuffer>(vertices, 3 * 4 * sizeof(double));
	EBO = std::make_unique<Drmix::OpenGL::IndexBuffer>(indices, 6);
	shader = std::make_unique<Drmix::OpenGL::Shader>("../../../../Mixture/ShaderToyMixture/ShaderToy.vert", "../../../../Mixture/ShaderToyMixture/ShaderToy.frag");

	Drmix::OpenGL::VertexBufferLayout layout;
	layout.push<double>(3);

	VAO->addBuffer(*VBO.get(), layout);
	shader->bind();
}

Canvas::~Canvas()
{

}

#include "RayTracingMixture.hpp"

RayTracingMixture::RayTracingMixture():
	m_VAO(nullptr),
	m_VBO(nullptr),
	m_EBO(nullptr),
	m_Shader(nullptr),
	m_Texture(nullptr)
{
	info.name = "RayTracing Mixture";
	info.resourcePath = "../../../../Mixture/RayTracingMixture";

	m_Canvas.resize(info.winWidth * info.winHeight * 3, 0);
}

RayTracingMixture::~RayTracingMixture()
{
	
}

void RayTracingMixture::onEnter()
{
	double vertices[] =
	{
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f
	};

	unsigned int indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};

	m_VAO = new Drmix::OpenGL::VertexArrayBuffer();
	m_VBO = new Drmix::OpenGL::VertexBuffer(vertices, (3 + 2) * 4 * sizeof(double));
	m_EBO = new Drmix::OpenGL::IndexBuffer(indices, 6);
	m_Shader = new Drmix::OpenGL::Shader("RayTracing.vert", "RayTracing.frag");
	m_Texture = new Drmix::OpenGL::Texture(m_Canvas.data(), info.winWidth, info.winHeight, 3);

	Drmix::OpenGL::VertexBufferLayout layout;
	layout.push<double>(3);
	layout.push<double>(2);

	m_VAO->addBuffer(*m_VBO, layout);
	
	m_Shader->bind();
	m_Shader->setUniform1i("Texture", 0);
}

void RayTracingMixture::update()
{

}

void RayTracingMixture::fixedUpdate(double deltaTime)
{
	for (size_t y = 0; y < info.winHeight; y++)
	{
		for (size_t x = 0; x < info.winWidth; x++)
		{

		}
	}
}

void RayTracingMixture::imGuiRender()
{

}

void RayTracingMixture::render()
{
	m_Texture->bind();
	mixtureRenderer.render(*m_VAO, *m_EBO, *m_Shader);
	m_Texture->unBind();
}

void RayTracingMixture::onExit()
{
	delete m_VAO;
	delete m_VBO;
	delete m_EBO;
	delete m_Shader;
}

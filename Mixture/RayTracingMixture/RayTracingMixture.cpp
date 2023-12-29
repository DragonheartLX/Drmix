#include "RayTracingMixture.hpp"

RayTracingMixture::RayTracingMixture():
	m_VAO(nullptr),
	m_VBO(nullptr),
	m_EBO(nullptr),
	m_Shader(nullptr),
	m_Texture(nullptr),
	m_RenderWidth(720),
    m_RenderHeigh(405),
	m_AspectRatio(16.0 / 9.0),
	m_ViewportHeight(2.0),
	m_ViewportWidth(m_ViewportHeight * (static_cast<double>(m_RenderWidth) / m_RenderHeigh)),
	m_FocalLength(1.0),
	m_CameraCenter(0, 0, 0),
	m_Viewport_U(m_ViewportWidth, 0, 0),
	m_Viewport_V(0, -m_ViewportHeight, 0),
	m_PixelDelta_U(m_Viewport_U / m_RenderWidth),
	m_PixelDelta_V(m_Viewport_V / m_RenderHeigh),
	m_ViewportUpperLeft(m_CameraCenter - Vec3(0, 0, m_FocalLength) - m_Viewport_U / 2 - m_Viewport_V / 2),
	m_Pixel00Loc(m_ViewportUpperLeft + 0.5 * (m_PixelDelta_U + m_PixelDelta_V))
{
	info.imGuiRender = true;
	info.name = "RayTracing Mixture";
	info.resourcePath = "../../../../Mixture/RayTracingMixture";

	m_Canvas.resize(m_RenderWidth * m_RenderHeigh * 3, 255);

	world.add(std::make_shared<Sphere>(Vec3(0, 0, -1), 0.5));
	world.add(std::make_shared<Sphere>(Vec3(0, -100.5, -1), 100));
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
	m_Texture = new Drmix::OpenGL::Texture(m_Canvas.data(), m_RenderWidth, m_RenderHeigh, 3);

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
	for (size_t y = 0; y < m_RenderHeigh; y++)
	{
		for (size_t x = 0; x < m_RenderWidth; x++)
		{
			Vec3 pixelCenter = m_Pixel00Loc + (x * m_PixelDelta_U) + (y * m_PixelDelta_V);
			Vec3 rayDirection = pixelCenter - m_CameraCenter;
			Ray r(m_CameraCenter, rayDirection);

			Vec3 color = rayColor(r, world);

			size_t index = (x + m_RenderWidth * (m_RenderHeigh - y - 1)) * 3;

			m_Canvas[index + 0] = static_cast<int>(255.999 * color[0]);
			m_Canvas[index + 1] = static_cast<int>(255.999 * color[1]);
			m_Canvas[index + 2] = static_cast<int>(255.999 * color[2]);
		}
	}

	m_Texture->updateTexture(m_Canvas.data(), m_RenderWidth, m_RenderHeigh, 3);
}

void RayTracingMixture::imGuiRender()
{
	ImGui::Text("FPS: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
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

Vec3 RayTracingMixture::rayColor(const Ray& r, const Object& world)
{
	HitRecord rec;
	if (world.hit(r, 0, infinity, rec))
	{
		return 0.5 * (rec.normal + Vec3(1, 1, 1));
	}

	Vec3 unitDirection = unit_vector(r.direction());
	auto a = 0.5 * (unitDirection.y() + 1.0);
	return (1.0 - a) * Vec3(1.0, 1.0, 1.0) + a * Vec3(0.5, 0.7, 1.0);
}

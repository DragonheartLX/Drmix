#include <Drmix.hpp>

#include "Math/Vec3.hpp"
#include "Ray/Ray.hpp"
#include "Object/Object.hpp"
#include "Utillty.hpp"

class RayTracingMixture: public Drmix::Mixture
{
public:
	RayTracingMixture();
	~RayTracingMixture();

	void onEnter() override;
	void update() override;
	void fixedUpdate(double deltaTime) override;
	void imGuiRender() override;
	void render() override;
	void onExit() override;

	Vec3 rayColor(const Ray& r, const Object& world);
private:
	Drmix::OpenGL::VertexArrayBuffer* m_VAO;
	Drmix::OpenGL::VertexBuffer* m_VBO;
	Drmix::OpenGL::IndexBuffer* m_EBO;
	Drmix::OpenGL::Shader* m_Shader;
	Drmix::OpenGL::Texture* m_Texture;
	std::vector<unsigned char> m_Canvas;
	int m_RenderWidth;
	int m_RenderHeigh;

	double m_AspectRatio;
	double m_ViewportHeight;
	double m_ViewportWidth;
	double m_FocalLength;
	Vec3 m_CameraCenter;

	Vec3 m_Viewport_U;
	Vec3 m_Viewport_V;

	Vec3 m_PixelDelta_U;
	Vec3 m_PixelDelta_V;

	Vec3 m_ViewportUpperLeft;
	Vec3 m_Pixel00Loc;

	ObjectList world;
};

Drmix::Mixture* Drmix::createMixture(int argc, char** argv)
{
	return new RayTracingMixture();
};
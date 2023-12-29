#include <Drmix.hpp>

class RayTracingMixture: public Drmix::Mixture
{
public:
	RayTracingMixture();
	~RayTracingMixture();

	/// <summary>
	/// 进入循环前执行一次
	/// </summary>
	void onEnter() override;

	/// <summary>
	/// 循环更新
	/// </summary>
	void update() override;

	/// <summary>
	/// 固定时间更新(每一次绘制前)
	/// </summary>
	/// <param name="deltaTime">距离上次绘制时间(second)</param>
	void fixedUpdate(double deltaTime) override;

	/// <summary>
	/// imGui绘制
	/// </summary>
	void imGuiRender() override;

	/// <summary>
	/// 主绘制函数
	/// </summary>
	void render() override;

	/// <summary>
	/// 退出前执行一次
	/// </summary>
	void onExit() override;

private:
	Drmix::OpenGL::VertexArrayBuffer* m_VAO;
	Drmix::OpenGL::VertexBuffer* m_VBO;
	Drmix::OpenGL::IndexBuffer* m_EBO;
	Drmix::OpenGL::Shader* m_Shader;
	Drmix::OpenGL::Texture* m_Texture;
	std::vector<unsigned char> m_Canvas;
};

/// <summary>
/// 将Mixture以指针形式传给Drmix
/// </summary>
/// <param name="argc">命令行参数个数</param>
/// <param name="argv">命令行参数数组</param>
/// <returns></returns>
Drmix::Mixture* Drmix::createMixture(int argc, char** argv)
{
	return new RayTracingMixture();
};
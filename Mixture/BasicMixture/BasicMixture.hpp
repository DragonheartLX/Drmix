#include <Drmix.hpp>

class BasicMixture: public Drmix::Mixture
{
public:
	BasicMixture();
	~BasicMixture();

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

};

/// <summary>
/// 将Mixture以指针形式传给Drmix
/// </summary>
/// <param name="argc">命令行参数个数</param>
/// <param name="argv">命令行参数数组</param>
/// <returns></returns>
Drmix::Mixture* Drmix::createMixture(int argc, char** argv)
{
	return new BasicMixture();
};
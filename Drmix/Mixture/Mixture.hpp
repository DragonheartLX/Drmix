#pragma once
#include <vector>
#include <functional>
#include <string>
#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Mixture
{
public:
	Mixture() {};
	virtual ~Mixture() {};

	virtual void event(GLFWwindow* win) {};
	virtual void update(double deltaTime) {};
	virtual void render() {};
	virtual void imguiRender() {};
private:

};

class MenuMixture: public Mixture
{
public:
	MenuMixture(Mixture*& mixture);
	~MenuMixture() {};

	void update(double deltaTime) override;
	void render() override;
	void imguiRender() override;

	template <typename T>
	void registerMixture(const std::string& name)
	{
		m_mixtures.push_back(std::make_pair(name, []() { return new T(); }));
		spdlog::info("Register Mixture: {0}", name);
	}

private:
	Mixture*& m_currentTest;
	std::vector<std::pair<std::string, std::function<Mixture*()>>> m_mixtures;
};
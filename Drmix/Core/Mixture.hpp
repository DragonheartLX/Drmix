#pragma once
#include <string>
#include <unordered_map>

#include "OpenGL/Renderer.hpp"

namespace Drmix
{
	struct MixtureInfo
	{
		std::string name = "Drmix Mixture";
		std::string resourcePath = "./";
		int winWidth = 1920;
		int winHeight = 1080;
		int framePerSecond = 90;
		bool mixtureShouldClose = false;
		bool imGuiRender = false;
	};

	class Mixture
	{
	public:
		Mixture();
		virtual ~Mixture();

		void init();
		void run();
		void release();

		virtual void onEnter() {};
		virtual void update() {};
		virtual void fixedUpdate(double deltaTime) {};
		virtual void imGuiRender() {};
		virtual void render() {};
		virtual void onExit() {};

		void setFramebufferSize(int width, int height);
		double getTime();

		MixtureInfo info;
		OpenGL::Renderer mixtureRenderer;
	private:

	};

	Mixture* createMixture(int argc, char** argv);
}
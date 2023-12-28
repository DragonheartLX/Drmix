#pragma once
#include <string>
#include <unordered_map>

#include "Object/Node.hpp"

namespace Drmix
{
	struct MixtureInfo
	{
		std::string name = "Drmix Mixture";
		int winWidth = 1920;
		int winHeight = 1080;
		int framePerSecond = 90;
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

		void render();
		void render(std::unordered_map<std::string, Node*> nodes);

		void setFramebufferSize(int width, int height);
		void setMixtureShouldClose(bool status = true);
		void setRenderImGui(bool status = true);
		double getTime();

		MixtureInfo info;
		std::unordered_map<std::string, Node*> objects;
	private:
		bool m_MixtureShouldClose;
		bool m_imGuiRender;
	};

	Mixture* createMixture(int argc, char** argv);
}
#pragma once
#include <Drmix.hpp>

class Canvas:public Drmix::Node
{
public:
	Canvas();
	~Canvas();
private:

};

class ShaderToyMixture:public Drmix::Mixture
{
public:
	ShaderToyMixture();
	~ShaderToyMixture();

	void onEnter() override;
	void update() override;
	void fixedUpdate(double deltaTime) override;
	void imGuiRender() override;
private:

};

Drmix::Mixture* Drmix::createMixture(int argc, char** argv)
{
	Drmix::MixtureInfo info;
	info.name = "ShaderToyMixture";
	Drmix::Mixture* mixture = new ShaderToyMixture();
	mixture->info = info;

	return mixture;
};
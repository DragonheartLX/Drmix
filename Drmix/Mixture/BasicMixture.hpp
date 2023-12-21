#pragma once
#include "Mixture.hpp"

class BasicMixture: public Mixture
{
public:
	BasicMixture();
	~BasicMixture() {};

	void imguiRender() override;
	void render() override;
private:
	float m_clearColor[3];
};
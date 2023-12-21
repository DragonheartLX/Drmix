#include "Mixture.hpp"
#include <imgui.h>

MenuMixture::MenuMixture(Mixture*& mixture):
	m_currentTest(mixture)
{
}


void MenuMixture::update(double deltaTime)
{
}

void MenuMixture::render()
{
}

void MenuMixture::imguiRender()
{
	for (std::pair<std::string, std::function<Mixture* ()>> mixture : m_mixtures)
	{
		if (ImGui::Button(mixture.first.c_str())) 
		{
			m_currentTest = mixture.second();
		}
	}
}
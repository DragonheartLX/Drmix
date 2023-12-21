#include "BasicMixture.hpp"
#include <imgui.h>
#include <glad/glad.h>

BasicMixture::BasicMixture():
	m_clearColor{ 0.16f, 0.16f, 0.16f}
{
}

void BasicMixture::imguiRender()
{
	ImGui::ColorEdit3("Clear color", m_clearColor);
}

void BasicMixture::render()
{
	glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

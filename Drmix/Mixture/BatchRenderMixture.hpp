#pragma once
#include "Mixture.hpp"
#include "OpenGL/OpenGL.hpp"

#include <memory>
#include <glm/fwd.hpp>
class BatchRenderMixture:public Mixture
{
public:
	BatchRenderMixture();
	~BatchRenderMixture();

	void update(double deltaTime) override;
	void render() override;
	void imguiRender() override;
private:
	std::unique_ptr<VertexArrayBuffer> m_VAO;
	std::unique_ptr<IndexBuffer> m_IndexBuffer;
	std::unique_ptr<VertexBuffer> m_VertexBuffer;
	std::unique_ptr<Shader> m_Shader;

	glm::mat4 m_Proj, m_View;
	glm::vec3 m_Translation;
};
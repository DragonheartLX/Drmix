#include "BatchRenderMixture.hpp"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

BatchRenderMixture::BatchRenderMixture():
    m_Proj(glm::ortho(0.0f, 1440.0f, 0.0f, 1080.0f, -1.0f, 1.0f)),
    m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
    m_Translation(glm::vec3(0, 0, 0))
{
    double positions[] = {
            100.0f, 100.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            200.0f, 100.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            200.0f, 200.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            100.0f, 200.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

            300.0f, 100.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
            400.0f, 100.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
            400.0f, 200.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
            300.0f, 200.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4
    };

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_VAO = std::make_unique<VertexArrayBuffer>();

    m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 8 * 8 * sizeof(double));
    VertexBufferLayout layout;
    layout.push<double>(4);
    layout.push<double>(4);
    m_VAO->addBuffer(*m_VertexBuffer, layout);

    m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 12);

    m_Shader = std::make_unique<Shader>("../../../Drmix/res/Shader/Batch.vert", "../../../Drmix/res/Shader/Batch.frag");
    m_Shader->enable();
}

BatchRenderMixture::~BatchRenderMixture()
{
}

void BatchRenderMixture::update(double deltaTime)
{
}

void BatchRenderMixture::render()
{
    Renderer renderer;
    glm::mat4 mvp = m_Proj * m_View;

    m_Shader->enable();
    m_Shader->setUniformMat4f("u_MVP", mvp);

    renderer.render(*m_VAO, *m_IndexBuffer, *m_Shader);
}

void BatchRenderMixture::imguiRender()
{
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

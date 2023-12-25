#include "Renderer.hpp"

void Renderer::render(const VertexArrayBuffer& va, const IndexBuffer& ib, const Shader& shader) const
{
    shader.enable();
    va.bind();
    ib.bind();

    glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr);
}
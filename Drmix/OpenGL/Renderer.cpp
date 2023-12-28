#include "OpenGL/Renderer.hpp"
#include "Core/Core.hpp"

#include <glad/glad.h>

namespace Drmix
{
	namespace OpenGL
	{
		Renderer::Renderer()
		{

		}

		Renderer::~Renderer()
		{

		}

		void Renderer::render(const VertexArrayBuffer& va, const IndexBuffer& ib, const Shader& shader) const
		{
			shader.bind();
			va.bind();
			ib.bind();

			GLCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
		}
	}
}
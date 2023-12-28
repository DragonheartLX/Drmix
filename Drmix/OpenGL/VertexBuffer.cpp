#include "OpenGL/VertexBuffer.hpp"
#include "Core/Core.hpp"

#include <glad/glad.h>

namespace Drmix
{
	namespace OpenGL
	{
		VertexBuffer::VertexBuffer(const void* data, unsigned int size)
		{
			GLCall(glGenBuffers(1, &m_RenderID));
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RenderID));
			GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW););
		}

		VertexBuffer::~VertexBuffer()
		{
			GLCall(glDeleteBuffers(1, &m_RenderID));
		}

		void VertexBuffer::bind() const
		{
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RenderID));
		}

		void VertexBuffer::unBind() const
		{
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
		}
	}
}
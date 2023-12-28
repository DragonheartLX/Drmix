#include "OpenGL/IndexBuffer.hpp"
#include "Core/Core.hpp"

#include <glad/glad.h>

namespace Drmix
{
	namespace OpenGL
	{
		IndexBuffer::IndexBuffer(unsigned int* data, unsigned int count):
			m_Count(count)
		{
			GLCall(glGenBuffers(1, &m_RenderID));
			GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID));
			GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
		}

		IndexBuffer::~IndexBuffer()
		{
			GLCall(glDeleteBuffers(1, &m_RenderID));
		}

		void IndexBuffer::bind() const
		{
			GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID));
		}

		void IndexBuffer::unBind() const
		{
			GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		}
	}
}
#include "OpenGL/VertexArrayBuffer.hpp"
#include "Core/Core.hpp"

#include <glad/glad.h>

namespace Drmix
{
	namespace OpenGL
	{
		VertexBufferLayout::VertexBufferLayout() :
			m_Stride(0)
		{
		}

		VertexBufferLayout::~VertexBufferLayout()
		{
		}

		template<typename T>
		void VertexBufferLayout::push(unsigned int count)
		{
			ASSERT(false);
		}

		template<>
		void VertexBufferLayout::push<double>(unsigned int count)
		{
			m_Elements.push_back({ GL_DOUBLE, count, GL_FALSE });
			m_Stride += VertexBufferElement::getSizeOfType(GL_DOUBLE) * count;
		}

		template<>
		void VertexBufferLayout::push<float>(unsigned int count)
		{
			m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
			m_Stride += VertexBufferElement::getSizeOfType(GL_FLOAT) * count;
		}

		template<>
		void VertexBufferLayout::push<unsigned int>(unsigned int count)
		{
			m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
			m_Stride += VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT) * count;
		}

		template<>
		void VertexBufferLayout::push<unsigned char>(unsigned int count)
		{
			m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
			m_Stride += VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE) * count;
		}

		template void VertexBufferLayout::push<double>(unsigned int count);
		template void VertexBufferLayout::push<float>(unsigned int count);
		template void VertexBufferLayout::push<unsigned int>(unsigned int count);
		template void VertexBufferLayout::push<unsigned char>(unsigned int count);

		VertexArrayBuffer::VertexArrayBuffer()
		{
			GLCall(glGenVertexArrays(1, &m_RenderID));
		}

		VertexArrayBuffer::~VertexArrayBuffer()
		{
			GLCall(glDeleteVertexArrays(1, &m_RenderID));
		}

		void VertexArrayBuffer::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) const
		{
			bind();
			vb.bind();
			const auto& elements = layout.getElements();
			unsigned int offset = 0;
			for (unsigned int i = 0; i < elements.size(); i++)
			{
				const auto& element = elements[i];

				GLCall(glEnableVertexAttribArray(i));
				GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void*)offset));
				offset += element.count * VertexBufferElement::getSizeOfType(element.type);
			}
		}

		void VertexArrayBuffer::bind() const
		{
			GLCall(glBindVertexArray(m_RenderID));
		}

		void VertexArrayBuffer::unBind() const
		{
			GLCall(glBindVertexArray(0));
		}

		unsigned int VertexBufferElement::getSizeOfType(unsigned int type)
		{
			switch (type)
			{
			case GL_DOUBLE:			return 8;
			case GL_FLOAT:			return 4;
			case GL_UNSIGNED_INT:	return 4;
			case GL_UNSIGNED_BYTE:	return 1;
			}
			ASSERT(false);
			return 0;
		}
	}
}
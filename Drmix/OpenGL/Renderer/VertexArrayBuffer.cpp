#include "VertexArrayBuffer.hpp"

VertexBufferLayout::VertexBufferLayout():
	m_Stride(0)
{
}

VertexBufferLayout::~VertexBufferLayout()
{
}

VertexArrayBuffer::VertexArrayBuffer()
{
	glGenVertexArrays(1, &m_RenderID);
}

VertexArrayBuffer::~VertexArrayBuffer()
{
	glDeleteVertexArrays(1, &m_RenderID);
}

void VertexArrayBuffer::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) const
{
	this->bind();
	vb.bind();
	const auto& elements = layout.getElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];

		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void*)offset);
		offset += element.count * VertexBufferElement::getSizeOfType(element.type);
	}
}

void VertexArrayBuffer::bind() const
{
	glBindVertexArray(m_RenderID);
}

void VertexArrayBuffer::unBind() const
{
	glBindVertexArray(0);
}

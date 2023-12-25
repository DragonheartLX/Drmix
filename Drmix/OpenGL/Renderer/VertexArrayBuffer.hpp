#pragma once
#include <glad/glad.h>
#include <vector>

#include "OpenGL/Renderer/VertexBuffer.hpp"

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int getSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_DOUBLE:			return 8;
		case GL_FLOAT:			return 4;
		case GL_UNSIGNED_INT:	return 4;
		case GL_UNSIGNED_BYTE:	return 1;
		}
		__debugbreak();
		return 0;
	}
};

class VertexBufferLayout
{
public:
	VertexBufferLayout();
	~VertexBufferLayout();

	template<typename T>
	void push(unsigned int count)
	{
		__debugbreak();
	}

	template<>
	void push<double>(unsigned int count)
	{
		m_Elements.push_back({ GL_DOUBLE, count, GL_FALSE });
		m_Stride += VertexBufferElement::getSizeOfType(GL_DOUBLE) * count;
	}

	template<>
	void push<float>(unsigned int count)
	{
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += VertexBufferElement::getSizeOfType(GL_FLOAT) * count;
	}

	template<>
	void push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT) * count;
	}

	template<>
	void push<unsigned char>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE) * count;
	}

	inline const std::vector<VertexBufferElement> getElements() const { return m_Elements; }
	inline unsigned int getStride() const { return m_Stride; }
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;
};

class VertexArrayBuffer
{
public:
	VertexArrayBuffer();
	~VertexArrayBuffer();
	
	void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) const;

	void bind() const;
	void unBind() const;
private:
	unsigned int m_RenderID;
};
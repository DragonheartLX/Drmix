#pragma once
#include "OpenGL/Renderer/VertexBuffer.hpp"
#include "OpenGL/Renderer/VertexArrayBuffer.hpp"
#include "OpenGL/Renderer/IndexBuffer.hpp"
#include "OpenGL/Shader/Shader.hpp"

class Renderer
{
public:
	void render(const VertexArrayBuffer& va, const IndexBuffer& ib, const Shader& shader) const;
};
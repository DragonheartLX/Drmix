#pragma once
#include "OpenGL/Shader.hpp"
#include "OpenGL/IndexBuffer.hpp"
#include "OpenGL/VertexArrayBuffer.hpp"

namespace Drmix
{
	namespace OpenGL
	{
		class Renderer
		{
		public:
			Renderer();
			~Renderer();

			void render(const VertexArrayBuffer& va, const IndexBuffer& ib, const Shader& shader) const;
		private:

		};
	}
}
#pragma once
#include "OpenGL/VertexBuffer.hpp"

#include <vector>

namespace Drmix
{
	namespace OpenGL
	{
		struct VertexBufferElement
		{
			unsigned int type;
			unsigned int count;
			unsigned char normalized;

			static unsigned int getSizeOfType(unsigned int type);
		};

		class VertexBufferLayout
		{
		public:
			VertexBufferLayout();
			~VertexBufferLayout();

			template<typename T>
			void push(unsigned int count);

			template<>
			void push<double>(unsigned int count);
			
			template<>
			void push<float>(unsigned int count);

			template<>
			void push<unsigned int>(unsigned int count);

			template<>
			void push<unsigned char>(unsigned int count);

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
	}
}
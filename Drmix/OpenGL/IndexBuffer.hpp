#pragma once
#include <memory>

namespace Drmix
{
	namespace OpenGL
	{
		class IndexBuffer
		{
		public:
			IndexBuffer(unsigned int* data, unsigned int count);
			~IndexBuffer();

			void bind() const;
			void unBind() const;

			inline unsigned int getCount() const { return m_Count; };
		private:
			unsigned int m_RenderID;
			unsigned int m_Count;
		};
	}
}
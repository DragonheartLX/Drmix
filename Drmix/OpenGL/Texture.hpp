#pragma once
#include <string>

namespace Drmix
{
	namespace OpenGL
	{
		class Texture
		{
		public:
			Texture();
			Texture(const std::string& path);
			Texture(unsigned char* data, int width, int height, int BPP);
			~Texture();

			void bind(unsigned int slot = 0) const;
			void unBind() const;

			void updateTexture(const std::string& path);
			void updateTexture(unsigned char* data, int width, int height, int BPP);

			void updateSubTexture(const std::string& path, int xoffset, int yoffset, int width, int height);
			void updateSubTexture(unsigned char* data, int xoffset, int yoffset, int width, int height);

			inline int getWidth() const { return m_Width; }
			inline int getHeight() const { return m_Height; }
		private:
			unsigned int m_RenderID;
			std::string m_FilePath;
			unsigned char* m_LocalBuffer;
			int m_Width, m_Height, m_BPP;
		};
	}
}
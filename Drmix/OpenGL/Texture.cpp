#include "OpenGL/Texture.hpp"
#include "Core/Core.hpp"
#include "Core/Filesystem.hpp"
#include "Core/Logger.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Drmix
{
	namespace OpenGL
	{
		Texture::Texture():
			m_RenderID(0), m_FilePath(""), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
		{
			GLCall(glGenTextures(1, &m_RenderID));
		}

		Texture::Texture(const std::string& path) :
			m_RenderID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
		{
			stbi_set_flip_vertically_on_load(1);
			m_LocalBuffer = stbi_load(Filesystem::getResourcePath(path).c_str(), &m_Width, &m_Height, &m_BPP, 4);

			GLCall(glGenTextures(1, &m_RenderID));
			GLCall(glBindTexture(GL_TEXTURE_2D, m_RenderID));

			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
			GLCall(glBindTexture(GL_TEXTURE_2D, 0));

			if (m_LocalBuffer) {
				stbi_image_free(m_LocalBuffer);
			}
		}

		Texture::Texture(unsigned char* data, int width, int height, int BPP):
			m_RenderID(0), m_FilePath(""), m_LocalBuffer(data), m_Width(width), m_Height(height), m_BPP(BPP)
		{
			GLCall(glGenTextures(1, &m_RenderID));
			GLCall(glBindTexture(GL_TEXTURE_2D, m_RenderID));

			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

			if (m_BPP == 3)
			{
				GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_LocalBuffer));
			}
			else if (m_BPP = 4)
			{
				GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
			}
			else
			{
				ASSERT(false);
			}
			
			GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		}

		Texture::~Texture()
		{
			GLCall(glDeleteTextures(1, &m_RenderID));
		}

		void Texture::bind(unsigned int slot) const
		{
			GLCall(glActiveTexture(GL_TEXTURE0 + slot));
			GLCall(glBindTexture(GL_TEXTURE_2D, m_RenderID));
		}

		void Texture::unBind() const
		{
			GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		}

		void Texture::updateTexture(const std::string& path)
		{
			m_FilePath = path;
			stbi_set_flip_vertically_on_load(1);
			int width, height, BPP;
			m_LocalBuffer = stbi_load(Filesystem::getResourcePath(path).c_str(), &width, &height, &BPP, 4);

			if (m_Width != width || m_Height != height || m_BPP != BPP)
			{
				GLCall(glDeleteTextures(1, &m_RenderID));

				m_Width = width;
				m_Height = height;
				m_BPP = BPP;

				GLCall(glGenTextures(1, &m_RenderID));
				GLCall(glBindTexture(GL_TEXTURE_2D, m_RenderID));

				GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
				GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
				GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
				GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

				if (m_BPP == 3)
				{
					GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_LocalBuffer));
				}
				else if (m_BPP = 4)
				{
					GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
				}
				else
				{
					ASSERT(false);
				}

				GLCall(glBindTexture(GL_TEXTURE_2D, 0));

				if (m_LocalBuffer) {
					stbi_image_free(m_LocalBuffer);
				}

				return;
			}
			
			GLCall(glBindTexture(GL_TEXTURE_2D, m_RenderID));

			GLCall(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
			GLCall(glBindTexture(GL_TEXTURE_2D, 0));

			if (m_LocalBuffer) {
				stbi_image_free(m_LocalBuffer);
			}
		}

		void Texture::updateTexture(unsigned char* data, int width, int height, int BPP)
		{
			m_FilePath = "";

			if (m_Width != width || m_Height != height || m_BPP != BPP)
			{
				GLCall(glDeleteTextures(1, &m_RenderID));

				m_Width = width;
				m_Height = height;
				m_BPP = BPP;

				GLCall(glGenTextures(1, &m_RenderID));
				GLCall(glBindTexture(GL_TEXTURE_2D, m_RenderID));

				GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
				GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
				GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
				GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

				GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
				GLCall(glBindTexture(GL_TEXTURE_2D, 0));

				return;
			}

			GLCall(glBindTexture(GL_TEXTURE_2D, m_RenderID));

			if (m_BPP == 3)
			{
				GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_LocalBuffer));
			}
			else if (m_BPP = 4)
			{
				GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
			}
			else
			{
				ASSERT(false);
			}

			GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		}

		void Texture::updateSubTexture(const std::string& path, int xoffset, int yoffset, int width, int height)
		{
			ASSERT(false);
			// TODO
			// Resolve resize
			m_FilePath = path;
			stbi_set_flip_vertically_on_load(1);
			m_LocalBuffer = stbi_load(Filesystem::getResourcePath(path).c_str(), &m_Width, &m_Height, &m_BPP, 4);

			GLCall(glBindTexture(GL_TEXTURE_2D, m_RenderID));

			GLCall(glTexSubImage2D(GL_TEXTURE_2D, 0, xoffset, yoffset, width, height, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
			GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		}

		void Texture::updateSubTexture(unsigned char* data, int xoffset, int yoffset, int width, int height)
		{
			ASSERT(false);
			// TODO
			// Resolve resize
			m_FilePath = "";
			m_LocalBuffer = data;

			GLCall(glBindTexture(GL_TEXTURE_2D, m_RenderID));

			GLCall(glTexSubImage2D(GL_TEXTURE_2D, 0, xoffset, yoffset, width, height, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
			GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		}
	}
}
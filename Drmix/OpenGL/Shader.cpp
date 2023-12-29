#include "OpenGL/Shader.hpp"
#include "Core/Core.hpp"
#include "Core/Filesystem.hpp"
#include "Core/Logger.hpp"

#include <fstream>
#include <glad/glad.h>

namespace Drmix
{
	namespace OpenGL
	{
		Shader::Shader(const std::string vertex, const std::string fragment)
		{
			unsigned int vert, frag;
			GLCall(m_ShaderID = glCreateProgram());
			vert = m_CompileShader(vertex, ShaderType::Vertex);
			frag = m_CompileShader(fragment, ShaderType::Fragment);

			GLCall(glAttachShader(m_ShaderID, vert));
			GLCall(glAttachShader(m_ShaderID, frag));
			GLCall(glLinkProgram(m_ShaderID));

			int status, length;
			GLCall(glGetProgramiv(m_ShaderID, GL_LINK_STATUS, &status));
			if (status == GL_FALSE)
			{
				GLCall(glGetShaderiv(m_ShaderID, GL_INFO_LOG_LENGTH, &length));
				char* msg = (char*)_malloca(length * sizeof(char));
				GLCall(glGetProgramInfoLog(m_ShaderID, length, &length, msg));
				DRMIX_LOG_WARN("Error: shader link error.");
				DRMIX_LOG_WARN(msg);
			}

			GLCall(glDeleteShader(vert));
			GLCall(glDeleteShader(frag));
		}

		Shader::~Shader()
		{
			GLCall(glDeleteProgram(m_ShaderID));
		}

		void Shader::bind() const
		{
			GLCall(glUseProgram(m_ShaderID));
		}

		void Shader::unBind() const
		{
			GLCall(glUseProgram(0));
		}

		void Shader::reload(const std::string vertex, const std::string fragment)
		{
			GLCall(glDeleteProgram(m_ShaderID));

			unsigned int vert, frag;
			GLCall(m_ShaderID = glCreateProgram());
			vert = m_CompileShader(vertex, ShaderType::Vertex);
			frag = m_CompileShader(fragment, ShaderType::Fragment);

			GLCall(glAttachShader(m_ShaderID, vert));
			GLCall(glAttachShader(m_ShaderID, frag));
			GLCall(glLinkProgram(m_ShaderID));

			int status, length;
			GLCall(glGetProgramiv(m_ShaderID, GL_LINK_STATUS, &status));
			if (status == GL_FALSE)
			{
				GLCall(glGetShaderiv(m_ShaderID, GL_INFO_LOG_LENGTH, &length));
				char* msg = (char*)_malloca(length * sizeof(char));
				GLCall(glGetProgramInfoLog(m_ShaderID, length, &length, msg));
				DRMIX_LOG_WARN("Error: shader link error.");
				DRMIX_LOG_WARN(msg);
			}

			GLCall(glDeleteShader(vert));
			GLCall(glDeleteShader(frag));
		}

		void Shader::setUniform1i(const std::string& name, int value)
		{
			GLCall(glUniform1i(m_GetUniformLocation(name), value));
		}

		void Shader::setUniform1f(const std::string& name, float value)
		{
			GLCall(glUniform1f(m_GetUniformLocation(name), value));
		}

		void Shader::setUniform1iv(const std::string& name, int count, int* value)
		{
			GLCall(glUniform1iv(m_GetUniformLocation(name), count, value));
		}

		void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
		{
			GLCall(glUniform4f(m_GetUniformLocation(name), v0, v1, v2, v3));
		}

		void Shader::setUniformMat4f(const std::string& name, const glm::mat4& matrix)
		{
			GLCall(glUniformMatrix4fv(m_GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
		}

		unsigned int Shader::m_CompileShader(std::string path, ShaderType type)
		{
			// Read source
			std::ifstream file(Filesystem::getResourcePath(path).c_str(), std::ios_base::in);
			if (!file.is_open())
			{
				DRMIX_LOG_WARN("Error: {0} shader file open error.", (type == ShaderType::Vertex ? "Vertex" : "Fragment"));
				file.close();
				return 0;
			}

			std::string line;
			std::string shader;
			while (std::getline(file, line))
				shader += line + "\n";

			const char* shaderSrc = shader.c_str();
			// Compile shader
			unsigned int shaderID;
			GLCall(shaderID = glCreateShader((type == ShaderType::Vertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER)));
			GLCall(glShaderSource(shaderID, 1, &shaderSrc, NULL));
			GLCall(glCompileShader(shaderID));

			// Check error
			int status, length;
			GLCall(glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status));
			if (status == GL_FALSE)
			{
				GLCall(glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length));
				char* msg = (char*)_malloca(length * sizeof(char));
				GLCall(glGetShaderInfoLog(shaderID, length, &length, msg));
				DRMIX_LOG_WARN("Error: {0} shader compile error.", (type == ShaderType::Vertex ? "Vertex" : "Fragment"));
				DRMIX_LOG_WARN(msg);
				GLCall(glDeleteShader(shaderID));
				return 0;
			}

			return shaderID;
		}

		unsigned int Shader::m_GetUniformLocation(const std::string& name)
		{

			return glGetUniformLocation(m_ShaderID, name.c_str());
		}
	}
}
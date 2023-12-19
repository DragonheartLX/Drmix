#include "Shader.hpp"
#include <fstream>
#include <spdlog/spdlog.h>
#include <glad/glad.h>

Shader::Shader(const std::string vertex, const std::string fragment)
{
	unsigned int vert, frag;
	m_shaderID = glCreateProgram();
	vert = m_compileShader(vertex, ShaderType::Vertex);
	frag = m_compileShader(fragment, ShaderType::Fragment);

	glAttachShader(m_shaderID, vert);
	glAttachShader(m_shaderID, frag);
	glLinkProgram(m_shaderID);

	int status, length;
	glGetShaderiv(m_shaderID, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		glGetShaderiv(m_shaderID, GL_INFO_LOG_LENGTH, &length);
		char* msg = (char*)_malloca(length * sizeof(char));
		glGetProgramInfoLog(m_shaderID, length, &length, msg);
		spdlog::warn("Error: shader link error.");
		spdlog::warn(msg);
	}

	glDeleteShader(vert);
	glDeleteShader(frag);
}

Shader::~Shader()
{
	glDeleteProgram(m_shaderID);
}

void Shader::enable()
{
	glUseProgram(m_shaderID);
}

void Shader::setUniform1i(const std::string& name, int value)
{
	glUniform1i(m_getUniformLocation(name), value);
}

void Shader::setUniform1f(const std::string& name, float value)
{
	glUniform1f(m_getUniformLocation(name), value);
}

void Shader::setUniform1iv(const std::string& name, int count, int* value)
{
	glUniform1iv(m_getUniformLocation(name), count, value);
}

void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	glUniform4f(m_getUniformLocation(name), v0, v1, v2, v3);
}

void Shader::setUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
	glUniformMatrix4fv(m_getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

unsigned int Shader::m_compileShader(std::string path, ShaderType type)
{
	// Read source
	std::ifstream file(path.c_str(), std::ios_base::in);
	if (!file.is_open())
	{
		spdlog::warn("Error: {0} shader file open error.", (type == ShaderType::Vertex ? "Vertex" : "Fragment"));
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
	shaderID = glCreateShader((type == ShaderType::Vertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER));
	glShaderSource(shaderID, 1, &shaderSrc, NULL);
	glCompileShader(shaderID);

	// Check error
	int status, length;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
		char* msg = (char*)_malloca(length * sizeof(char));
		glGetShaderInfoLog(shaderID, length, &length, msg);
		spdlog::warn("Error: {0} shader compile error.", (type == ShaderType::Vertex ? "Vertex" : "Fragment"));
		spdlog::warn(msg);
		glDeleteShader(shaderID);
		return 0;
	}

	return shaderID;
}

unsigned int Shader::m_getUniformLocation(const std::string& name)
{

	return glGetUniformLocation(m_shaderID, name.c_str());
}

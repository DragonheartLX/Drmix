#pragma once
#include <string>
#include <glm/glm.hpp>
enum ShaderType
{
	None,
	Vertex,
	Fragment
};

class Shader
{
public:
	Shader(const std::string vertex, const std::string fragment);
	~Shader();

	void enable();

	void setUniform1i(const std::string& name, int value);
	void setUniform1f(const std::string& name, float value);
	void setUniform1iv(const std::string& name, int count, int* value);
	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void setUniformMat4f(const std::string& name, const glm::mat4& matrix);
private:
	unsigned int m_shaderID;

	unsigned int m_compileShader(std::string path, ShaderType type);
	unsigned int m_getUniformLocation(const std::string& name);
};